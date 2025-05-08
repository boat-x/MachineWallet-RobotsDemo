/**
****************************************************************************************
* @FilePath: boat_ssl.c
* @Author: aitos
* @Date: 2023-02-15 11:49:33
* @LastEditors:
* @LastEditTime: 2023-02-15 11:49:33
* @Descripttion:
****************************************************************************************
*/
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"
#include "httpclient.h"
#include "fibo_opencpu.h"
#include "boatplatformdal.h"


/**
****************************************************************************************
* @brief:
*  This function connect to address.
* @param[in] *address
*  URL of the network wanted to connect.
* @param[in] *rsvd
*  unused untill now
* @return
*  This function will return socketID if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{
    int connectfd;
    char ip[64];
    char port[8];
    char *ptr = NULL;
    // struct hostent *he;
    // struct sockaddr_in server;
    // struct sockaddr localaddr;
    // struct sockaddr_in *localaddr_ptr;
    // socklen_t addrlen = sizeof(struct sockaddr);

    (void)rsvd;

    ptr = strchr(address, ':');
    if (NULL == ptr)
    {
        BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address);
        return -1;
    }

    memset(ip, 0, sizeof(ip));
    memset(port, 0, sizeof(port));
    memcpy(ip, address, (int)(ptr - address));
    memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

#if (BOAT_TLS_SUPPORT == 1)
    connectfd = fibo_ssl_sock_create();
    if (connectfd == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }
    if (fibo_ssl_sock_connect(connectfd, ip, atoi(port)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error ,code = %d", fibo_get_ssl_errcode());
        fibo_ssl_sock_close(connectfd);
        return -1;
    }
    /// get ssl socket fd
    int fd = fibo_ssl_sock_get_fd(connectfd);

    // set sock fd to unblock
    fibo_sock_lwip_fcntl(fd, F_SETFL, fibo_sock_lwip_fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);
#else
    GAPP_TCPIP_ADDR_T addr;
    if (getHostByIpOrName(address, &addr) != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS");
        return HTTPCLIENT_UNRESOLVED_DNS;
    }

    addr.sin_port = boat_htons(/*7545*/ client->remote_port);
    if (client->socket < 0)
    {
        return HTTPCLIENT_ERROR_CONN;
    }

    if ((connectfd = fibo_sock_create(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }

    if (fibo_sock_connect(connectfd, &addr) == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error");
        fibo_ssl_sock_close(connectfd);
        return -1;
    }
#endif

    return connectfd;
}

#if (BOAT_TLS_SUPPORT == 1)
/**
****************************************************************************************
* @brief:
* This function initialize TLS connection. This initialization will set hostname and CA cert of
* server's TLS cert or client's TLS cert or key.
* This function will connect to server and complete HandShake process.
* @param[in] *address
* URL of server network
* @param[in] *hostName
* CN of server's TLS cert
* @param[in] caChain
* CA cert of server's TLS cert
* @param[in] clientPrikey
* client's tls prikey
* @param[in] clientCert
* client's tls cert
* @param[out] *socketfd
* socketfd of the connection between client and server
* @param[out] **tlsContext
* output of tls context
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return BOAT_SUCCESS if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
                        const BoatFieldVariable clientCert, BSINT32 *socketfd, boatSSlCtx **tlsContext, void *rsvd)
{

    int ret = 0;

    fibo_set_ssl_chkmode(1);
    ret = fibo_write_ssl_file("TRUSTFILE", caChain.field_ptr, caChain.field_len);
    if (BOAT_TLS_IDENTIFY_CLIENT == 1)
    {
        ret = fibo_write_ssl_file("CAFILE", clientCert.field_ptr, clientCert.field_len);
        ret = fibo_write_ssl_file("CAKEY", clientPrikey.field_ptr, clientPrikey.field_len);
    }
    ret = BoatConnect(address, NULL);
    if (ret == -1)
    {
        BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
        return BOAT_ERROR;
    }
    *socketfd = ret;
    // fibo_taskSleep(10000);
    BoatLog(BOAT_LOG_NORMAL, " BoatTlsInit OK");
    return BOAT_SUCCESS;
}
#endif

/**
****************************************************************************************
* @brief:
* This function send data with length equal to len to server.
* @param[in] sockfd
* socketfd of this connection.
* @param[in] *tlsContext
* tls context between client and server
* @param[in] *buf
* point to the data wanted to send
* @param[in] len
* length of the data wanted to send
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return length of sent data if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatSend(BSINT32 sockfd, boatSSlCtx *tlsContext, const BUINT8 *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    BSINT32 ret = fibo_ssl_sock_send(sockfd, (UINT8 *)buf, len);
    // BoatLog(BOAT_LOG_VERBOSE, "write ssl send = %d ", ret);
    return ret;
#else
    return send(sockfd, buf, len, 0);
#endif
}

static int ssl_recv_unblock(INT32 sock, void *buf, INT32 size, INT32 timeout)
{
    struct timeval tm = {0};
    fd_set rset;
    // BUINT8 *temp = buf;

    int fd = fibo_ssl_sock_get_fd(sock);

    int ret = fibo_ssl_sock_recv(sock, buf, size);
    if (ret > 0)
    {
        // BoatLog(BOAT_LOG_VERBOSE, "recv data size:%d", ret);
        return ret;
    }
    else if (ret < 0)
    {
        // BoatLog(BOAT_LOG_VERBOSE, "recv data fail");
        return ret;
    }

    FD_ZERO(&rset);
    FD_SET(fd, &rset);
    tm.tv_sec = timeout / 1000;
    tm.tv_usec = (timeout % 1000) * 1000;

    ret = select(fd + 1, &rset, NULL, NULL, timeout > 0 ? &tm : NULL);
    if (ret < 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "select failed:%s", strerror(errno));
        return -1;
    }
    else if (ret == 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "select timeout");
        return -1;
    }
    else
    {
        ret = fibo_ssl_sock_recv(sock, buf, size);
        if (ret >= 0)
        {
            // BoatLog(BOAT_LOG_VERBOSE, "recv data size:%d", ret);
            return ret;
        }
        else if (ret < 0)
        {
            BoatLog(BOAT_LOG_VERBOSE, "recv data fail");
            return ret;
        }
    }
    return -1;
}

/**
****************************************************************************************
* @brief:
* This function receive data from server
* @param[in] sockfd
* socketfd of this connection.
* @param[in] *tlsContext
* tls context between client and server
* @param[in] *buf
* point to the data wanted to receive
* @param[in] len
* length of the buf to store receiving data
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return length of received data if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatRecv(BSINT32 sockfd, boatSSlCtx *tlsContext, BUINT8 *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    BSINT32 ret = ssl_recv_unblock(sockfd, buf, len, 10 * 1000);
    return ret;
#else
    return recv(sockfd, buf, len, 0);
#endif
}
/**
****************************************************************************************
* @brief:
* This function close the connection between client and server.
* This function must release the tls context.
* @param[in] sockfd
* socketfd of this connection.
* @param[in] **tlsContext
* tls context between client and server
* @param[in] *rsvd
* unused untill now
* @return
* This function has no returned value.
****************************************************************************************
*/
void BoatClose(BSINT32 sockfd, boatSSlCtx **tlsContext, void *rsvd)
{
    // close(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
    // free tls releated
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    fibo_ssl_sock_close(sockfd);
#else
    close(sockfd);
#endif
}
