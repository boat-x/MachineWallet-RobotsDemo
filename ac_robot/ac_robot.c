/******************************************************************************
Copyright (C) TLAY.IO

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
����
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "fibofwk.h"
#include "fibo_info_interface.h"
#include "fibo_mdc_interface.h"
#include "fibo_mrc_interface.h"
#include "fibo_sim_interface.h"
#include "fibo_mcc_interface.h"
#include "fibo_print.h"
#include "fibo_aps_interface.h"
#include "fibo_atClient_interface.h"
#include "fibo_atDefs_interface.h"


#include "curl/curl.h"
#include "fibo_semaphore.h"
#include "fibo_timer.h"
#include "fibo_print.h"
#include "fibo_http.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "erc20Transfer.h"
#include "boatlog.h"
#include "web3intf.h"

#define DEFAULT_PROFILE_INDEX 1

/******************************************************************************
 Before first-time compilation, create a file named credentials.key in the same
 directory as this one, with below content:
 
 // Create credentials.key with necessary keys.
 // This file MUST be kept locally. DO NOT upload it to Internet.
 
 // Private key of A/C Service Robot:
 const BCHAR *g_ac_robot_key = "0x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef";
 
 // Address of A/C Service Robot:
 const BCHAR *g_ac_robot_address = "0x1234567890abcdef1234567890abcdef12345678";

 // Address of Investor David:
 const BCHAR *g_investor_david_address = "0x2234567890abcdef1234567890abcdef12345678";

 // Address of Investor Charlie:
 const BCHAR *g_investor_charlie_address = "0x3234567890abcdef1234567890abcdef12345678";

 // Command to obtain access token from OWON
 BCHAR * owon_obtain_token_cmd = "{\"agentId\":\"1234567890abcdef1234567890123456\",\"agentKey\":\"abcdef123\",\"ts\":\"586851710\"}";

 // Command to swith on the plug
 BCHAR * owon_switch_off_cmd = "{\"agentId\": \"1234567890abcdef1234567890123456\",\"mac\": \"1234567890ABCDEF\",\"accessToken\": \"%s\",\"sjson\":\"{ \\\"session\\\":\\\"\\\", \\\"type\\\":\\\"smartPlug\\\", \\\"sequence\\\":1017, \\\"command\\\":\\\"setSwitchgear\\\", \\\"argument\\\": { \\\"ieee\\\":\\\"FECDAB9078563411\\\", \\\"ep\\\":1, \\\"switchgear\\\": \\\"off\\\" } }\",\"ts\": \"1742372986\"}";

 // Command to swith off the plug
 BCHAR * owon_switch_on_cmd = "{\"agentId\": \"1234567890abcdef1234567890123456\",\"mac\": \"1234567890ABCDEF\",\"accessToken\": \"%s\",\"sjson\":\"{ \\\"session\\\":\\\"\\\", \\\"type\\\":\\\"smartPlug\\\", \\\"sequence\\\":1017, \\\"command\\\":\\\"setSwitchgear\\\", \\\"argument\\\": { \\\"ieee\\\":\\\"FECDAB9078563411\\\", \\\"ep\\\":1, \\\"switchgear\\\": \\\"on\\\" } }\",\"ts\": \"1742372986\"}";

******************************************************************************/
#include "credentials.key"


static fibo_mdc_ProfileRef_t g_profileRef = NULL;


const BCHAR * g_rpcUrl = "https://base-mainnet.infura.io";
const BUINT32 g_chain_id = 8453;

const BCHAR * g_erc20_contract_address_str = "0x833589fCD6eDb6E08f4c7C32D4f71b54bdA02913"; // USDC on Base mainnet

BoatEthWallet *g_ethereum_wallet_ptr;
BUINT8 g_keypairIndex = 0;
BUINT8 g_networkIndex = 0;

BUINT32 g_banlanceOf = 0;
BCHAR g_onoff = 0;

// Declaration

void *ReadUARTThreadEntry(void *arg);

#define DEFAULT_PROFILE_INDEX 1


static fibo_mdc_ProfileRef_t profileRef = NULL;

static fibo_sem_Ref_t   waitSem = NULL;  


BOAT_RESULT ethereum_createKeypair(const BCHAR *nativePrivateKey, BCHAR * keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

	/* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, nativePrivateKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = binFormatKey;
    keypair_config.prikey_content.field_len = 32;


	/* create ethereum keypair */
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_RAM);

    if (result < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_keypairIndex = result;
    
    return BOAT_SUCCESS;
}

BOAT_RESULT createEthereumNetwork()
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatEthNetworkConfig network_config = {0};

    network_config.chain_id             = g_chain_id;
    network_config.eip155_compatibility = BOAT_TRUE;
    strncpy(network_config.node_url_str, g_rpcUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

    result = BoATEthNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);

    if (result < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    g_networkIndex = result;
    
    return BOAT_SUCCESS;
}


/*!****************************************************************************
 * @brief Transfer ERC20
 *
 * @details
 *   This function transfers ERC20 token to specified to_address.

 * @param[in] wallet_ptr
 *   The wallet pointer that this transaction is combined with.
 *
 * @param[in] erc20_contract_address_str
 *   A 0x-prefix string representing the ERC20 contract address in HEX format.
 *
 * @param[in] to_address
 *   A 0x-prefix string representing the to_address in HEX format.
 *
 * @param[in] transfer_value
 *   The value to transfer. The value unit depends on the ERC-20's decimals.\n
 *   For example, USDC has 6 decimals, the value unit is 1E-6 USDC.\n
 *   Hence, value 10000 means 10000E-6 = 0.01USDC.
 *
 *	
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns one of the error codes.
 *
*******************************************************************************/
BOAT_RESULT call_erc20_transfer(BoatEthWallet *wallet_ptr, const BCHAR *erc20_contract_address_str, const BCHAR *to_address_str, BUINT32 transfer_value)
{
    BCHAR *result_str;
    BoatEthTx tx_ctx;
    BOAT_RESULT result;
    
    /* Set Contract Address */
    result = BoatEthTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   erc20_contract_address_str);

    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "BoatEthTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    

    BoatAddress to_address;
    BUINT32 ret;
    ret = UtilityHexToBin(to_address, sizeof(to_address), to_address_str, TRIMBIN_TRIM_NO, BOAT_FALSE);
    if( ret != sizeof(to_address))
    {
        return BOAT_ERROR;
    }
    UtilityChangeEndian(to_address, sizeof(to_address));

    BUINT256 value256;
    memset(value256, 0, sizeof(BUINT256));
    value256[0] = transfer_value & 0xFF;
    value256[1] = (transfer_value >> 8) & 0xFF;
    value256[2] = (transfer_value >> 16) & 0xFF;
    value256[3] = (transfer_value >> 24) & 0xFF;
    
    result_str = erc20Transfer_transfer(&tx_ctx, to_address, value256);

    
    if (result_str != NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERC-20 transfer returns: %s", result_str);
        
        return BOAT_SUCCESS;
    }
    else
    {
        return BOAT_ERROR;
    }
}


/*!****************************************************************************
 * @brief Read ERC20 balance
 *
 * @details
 *   This function reads the ERC20 balance of a specified owner_address.

 * @param[in] wallet_ptr
 *   The wallet pointer that this transaction is combined with.
 *
 * @param[in] erc20_contract_address_str
 *   A 0x-prefix string representing the ERC20 contract address in HEX format.
 *
 * @param[in] owner_address_str
 *   A 0x-prefix string representing the owner_address in HEX format.
 *
 * @param[in] transfer_value
 *   The value to transfer. The value unit depends on the ERC-20's decimals.\n
 *   For example, USDC has 6 decimals, the value unit is 1E-6 USDC.\n
 *   Hence, value 10000 means 10000E-6 = 0.01USDC.
 *
 *	
 * @return
 *   This function returns the balance (up to 2^32-1) of owner_address in ERC20.\n
 *   The value unit depends on the ERC-20's decimals.\n
 *   For example, USDC has 6 decimals, the value unit is 1E-6 USDC.\n
 *   Hence, value 10000 means 10000E-6 = 0.01USDC.\n
 *   If any error occurs, it returns zero.
 *
*******************************************************************************/
BUINT32 call_erc20_balanceOf(BoatEthWallet *wallet_ptr, const BCHAR *erc20_contract_address_str, const BCHAR *owner_address_str)
{
    BCHAR *result_str;
    BoatEthTx tx_ctx;
    BOAT_RESULT result;

    /* Set Contract Address */
    result = BoatEthTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   erc20_contract_address_str);

    if (result != BOAT_SUCCESS)
	{
        BoatLog(BOAT_LOG_NORMAL, "BoatEthTxInit fails.");
        return 0;
    }
    
    BoatAddress owner_address;
    BUINT32 ret;
    ret = UtilityHexToBin(owner_address, sizeof(owner_address), owner_address_str, TRIMBIN_TRIM_NO, BOAT_FALSE);
    if( ret != sizeof(owner_address))
    {
        return 0;
    }
    UtilityChangeEndian(owner_address, sizeof(owner_address));
    
    result_str = erc20Transfer_balanceOf(&tx_ctx, owner_address);


    BoatFieldVariable parse_result;
    BUINT256 balance256;
    BUINT32 balance32;
    parse_result.field_ptr = BoatMalloc(512);
    if( parse_result.field_ptr != NULL )
    {
        parse_result.field_len = 512;
    }
    else
    {
        return 0;
    }

    result = BoatEthParseRpcResponseStringResult(result_str, &parse_result);
    if (result == BOAT_SUCCESS && result_str != NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERC20 balanceOf returns: %s", result_str);
        
        UtilityHexToBin(balance256, sizeof(balance256),
						(BCHAR*)parse_result.field_ptr, TRIMBIN_TRIM_NO, BOAT_FALSE);
        UtilityChangeEndian(balance256, sizeof(balance256));
        
        *((BUINT8*)&balance32) = balance256[0];
        *((BUINT8*)&balance32+1) = balance256[1];
        *((BUINT8*)&balance32+2) = balance256[2];
        *((BUINT8*)&balance32+3) = balance256[3];
        
        BoatLog(BOAT_LOG_NORMAL, "balance is %u unit", balance32);
        
        BoatFree(parse_result.field_ptr);
        parse_result.field_ptr = NULL;
        parse_result.field_len = 0;
        return balance32;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Error: cannot parse balance");
        BoatFree(parse_result.field_ptr);
        parse_result.field_ptr = NULL;
        parse_result.field_len = 0;
        return 0;
    }

}





static void ConnectionStateHandler
(
    fibo_mdc_ProfileRef_t profileRef,
    fibo_mdc_ConState_t state,
    void* contextPtr
)
{
    FIBO_INFO("state=%d", state);
    if(state == 2)
    {
        fibo_sem_Post(waitSem);
    }
}

static void* DataTestThread
(
    void* context
)
{
    fibo_mdc_ConnectService();
    
    fibo_mdc_ProfileRef_t profileRef = (fibo_mdc_ProfileRef_t)context;

    // Add state handler on the profile
    fibo_mdc_AddSessionStateHandler(profileRef, ConnectionStateHandler, NULL);

    // Run the event loop
    fibo_event_RunLoop();
}

size_t fibo_http_callback
(
    void *buffer, 
    size_t size, 
    size_t nmemb, 
    void *stream
)
{
    if(NULL != buffer)
    {
        printf("nmemb: %d, buffer: %s\n", nmemb, (char *)buffer);
    }

    return (size * nmemb);
}




BOAT_RESULT ActivateNetwork(void)
{
    
    // Check the current state of the cid
    fibo_mdc_ConState_t mdc_state = FIBO_MDC_DISCONNECTED;
    fibo_mrc_NetRegState_t mrc_state = FIBO_MRC_REG_UNKNOWN;
    fibo_result_t     res;
    char itfName[FIBO_MDC_INTERFACE_NAME_MAX_BYTES] = {0};
    char ipAddr[100] = {0};

    fibo_mdc_ConnectService();
    fibo_mrc_ConnectService();

    g_profileRef = fibo_mdc_GetProfile(DEFAULT_PROFILE_INDEX);
    if(!g_profileRef)
    {
        FIBO_INFO("fibo_mdc_GetProfile return NULL");
        return BOAT_ERROR;
    }

    while(1)
    {
        res = fibo_mrc_GetNetRegState(&mrc_state);
        FIBO_INFO("fibo_mrc_GetNetRegState result:%s", FIBO_RESULT_TXT(res));

        if( res != FIBO_OK )
        {
            return BOAT_ERROR;
        }
        else
        {
            if( mrc_state == FIBO_MRC_REG_HOME || mrc_state == FIBO_MRC_REG_ROAMING )
            {
                break;
            }
            else
            {
                sleep(5);
            }
            
        }
        
    }
    
   
    // Check the state
    res = fibo_mdc_GetSessionState(g_profileRef, &mdc_state);
    FIBO_INFO("fibo_mdc_GetSessionState state => %d, result:%s", mdc_state, FIBO_RESULT_TXT(res));
   
    // If already connected, disconnect the session
    if ( mdc_state == FIBO_MDC_CONNECTED )
    {
        res = fibo_mdc_StopSession(g_profileRef);
        FIBO_INFO("fibo_mdc_StopSession result:%s", FIBO_RESULT_TXT(res));
    }

    res = fibo_mdc_SetPDP(g_profileRef, FIBO_MDC_PDP_IPV4);
    FIBO_INFO("fibo_mdc_SetPDP result:%s", FIBO_RESULT_TXT(res));

    res = fibo_mdc_SetAPN(g_profileRef, "UNIM2M.NJM2MAPN");
    FIBO_INFO("fibo_mdc_SetAPN result:%s", FIBO_RESULT_TXT(res));

    res = fibo_mdc_StartSession(g_profileRef);
    FIBO_INFO("fibo_mdc_StartSession result:%s", FIBO_RESULT_TXT(res));

    return BOAT_SUCCESS;
}


BOAT_RESULT DeactivateNetwork(void)
{
    fibo_result_t     res;
    BOAT_RESULT result;

    res = fibo_mdc_StopSession(g_profileRef);
    FIBO_INFO("fibo_mdc_StopSession result:%s", FIBO_RESULT_TXT(res));

    fibo_mdc_DisconnectService();
    fibo_mrc_DisconnectService();


    if( res == FIBO_OK )    result = BOAT_SUCCESS;
    else result = BOAT_ERROR;

    return result;
}






char hexstringToChar(char *p)
{
	char out = 0;
	if(p[0] >= 'a')
		out = p[0] - 'a' + 10;
	else if(p[0] >= 'A')
		out = p[0] - 'A' + 10;
	else
		out = p[0] - '0';

	out = out * 0x10;

	if(p[1] >= 'a')
		out += p[1] - 'a' + 10;
	else if(p[1] >= 'A')
		out += p[1] - 'A' + 10;
	else
		out += p[1] - '0';

	return out;
}


void checkBanlanceLoop()
{
    BUINT32 banlanceOf;
    BCHAR *p;

    g_banlanceOf = call_erc20_balanceOf(g_ethereum_wallet_ptr, g_erc20_contract_address_str, g_ac_robot_address);
    
	BoatLog(BOAT_LOG_NORMAL, "\n  Initial banlance: %u\n", g_banlanceOf);
	

	while(1)
	{
	    banlanceOf = call_erc20_balanceOf(g_ethereum_wallet_ptr, g_erc20_contract_address_str, g_ac_robot_address);
	    
		if(banlanceOf != 0)
		{

			printf("   banlanceof, g_banlanceOf: %u, %u\n", banlanceOf, g_banlanceOf);

			if(banlanceOf > g_banlanceOf)
			{
				if(g_banlanceOf == 0)
				{
					g_banlanceOf = banlanceOf;
				}
				else if((banlanceOf - g_banlanceOf) >= 100000)  // 100000 is 0.01USDC
				{
					//if(g_onoff ==0)
					{
					    p = web3_eth_sendRestfulCallWithJsonItem(g_ethereum_wallet_ptr->web3intf_context_ptr,
                               "http://gn1.owon-care.com:3330/busproc/nt/agent/getaccesstoken",
							owon_obtain_token_cmd,
							"accessToken");
						if(p != NULL)
						{
							printf("\n   accessToken: %s\n", p);
						
							char onOffBuf[500] = {0};
							snprintf(
             					onOffBuf,
								sizeof(onOffBuf),
								owon_switch_on_cmd,
								p             
								);
							///printf("\n   onOffBuf:%s\n", onOffBuf);
							p = web3_eth_sendRestfulCallWithJsonItem(g_ethereum_wallet_ptr->web3intf_context_ptr,
                               "http://gn1.owon-care.com:3330/busproc/agent/sendGwData",
								onOffBuf,
								"code");
							if(p != NULL)
							{
								printf("\n   on code: %s\n", p);
							}
						}
							g_onoff = 8;


                        BUINT32 delta = banlanceOf - g_banlanceOf;
                        BUINT32 value;

                        // Send 50% of the income to David
                        value = delta*50/100;
                        BoatLog(BOAT_LOG_NORMAL, "   David account: %u", value);
                        call_erc20_transfer(g_ethereum_wallet_ptr, g_erc20_contract_address_str, g_investor_david_address, value);

                        sleep(10);

                        // Send 30% of the income to Charlie
                        value = delta*30/100;
                        BoatLog(BOAT_LOG_NORMAL, "   Charlie account: %u", value);
                        call_erc20_transfer(g_ethereum_wallet_ptr, g_erc20_contract_address_str, g_investor_charlie_address, value);

                        // The remaining 20% of the income is for the robot itself
                        
                        // Update AC Robot's balance
                        g_banlanceOf = call_erc20_balanceOf(g_ethereum_wallet_ptr, g_erc20_contract_address_str, g_ac_robot_address);
                        BoatLog(BOAT_LOG_NORMAL, "   Update g_banlanceof after revenue is shared: %d\n", g_banlanceOf);

					}
					
					
				}
				
			}
		
		}
		if(g_onoff > 0)
		{
			if(g_onoff == 1)
			{
				p = web3_eth_sendRestfulCallWithJsonItem(g_ethereum_wallet_ptr->web3intf_context_ptr,
				        "http://gn1.owon-care.com:3330/busproc/nt/agent/getaccesstoken",
					owon_obtain_token_cmd,
					"accessToken");
				if(p != NULL)
				{
					printf("\n   accessToken: %s\n", p);
				
					char offOnBuf[500] = {0};
					snprintf(
						offOnBuf,
						sizeof(offOnBuf),
						owon_switch_off_cmd,
						p			  
						);
					///printf("\n	 onOffBuf:%s\n", offOnBuf);
					p = web3_eth_sendRestfulCallWithJsonItem(g_ethereum_wallet_ptr->web3intf_context_ptr,
					        "http://gn1.owon-care.com:3330/busproc/agent/sendGwData",
						offOnBuf,
						"code");
					if(p != NULL)
					{
						printf("\n   off code: %s\n", p);
					}
				}
			}
			g_onoff--;
		}
		sleep(10);
	}
}


int main(int argc, char *argv[])
{
    BOAT_RESULT result;

    // Usage Example: boatapp http://127.0.0.1:7545
    /*
    if( argc != 2 )
    {
        BoatLog(BOAT_LOG_CRITICAL, "uart test Usage: %s http://<IP Address or URL for node>:<port>\n", argv[0]);
        return BOAT_ERROR;
    }
	*/
    fibo_arg_SetArgs((size_t)argc, (char**)argv);
    setlinebuf(stdout);
    
    // Register the component with the Log Daemon.
    FIBO_INFO("uart test== Starting Executable '%s' ==", STRINGIZE(FIBO_EXECUTABLE_NAME));

    // Connect to the log control daemon.
    // Note that there are some rare cases where we don't want the
    // process to try to connect to the Log Control Daemon (e.g.,
    // the Supervisor and the Service Monitor shouldn't).
    // The NO_LOG_CONTROL macro can be used to control that.
#ifndef NO_LOG_CONTROL
        fibo_log_ConnectToControlDaemon();
#else
        FIBO_DEBUG("Not connecting to the Log Control Daemon.");
#endif
	curl_version_info_data *  civd = curl_version_info(CURLVERSION_FIRST);
    printf("\ncurl version: %s\n", civd->version);
    FIBO_INFO("Start oemDataTest");

	fibo_SetAppLimits(STRINGIZE(FIBO_EXECUTABLE_NAME));

	fibo_SetAppNotifyOnRelease(STRINGIZE(FIBO_EXECUTABLE_NAME));


    
    ActivateNetwork();

    fibo_atClient_ConnectService();
	//waitSem = fibo_sem_Create("WaitSignal",0);

    //// BoAT Inintialization
    
     /* Boat SDK initialization */
    BoatIotSdkInit();


    /* Create Ethereum-compatible Network */
    BoatLog(BOAT_LOG_NORMAL,"    execute create network");
    result = createEthereumNetwork();
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed: %d.", result);
		//return -1;
        goto main_destruct;
	}


    /* Create ethereum wallet */
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = ethereum_createKeypair(g_ac_robot_key, "keypair00");
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed: %d.", result);
		goto main_destruct;
	}

    BoatLog(BOAT_LOG_NORMAL,"    execute wallet init");
    g_ethereum_wallet_ptr = BoatEthWalletInit(g_keypairIndex,g_networkIndex);
    if(g_ethereum_wallet_ptr == NULL){
        // BoatLog(BOAT_LOG_NORMAL,"BoatEthWalletInit fail");
        goto main_destruct;
    }


    BoatLog(BOAT_LOG_NORMAL, "====== UARTTrace ======");
 	//sleep(30);

    checkBanlanceLoop();


main_destruct:

    fibo_atClient_DisconnectService();
    DeactivateNetwork();
    BoatIotSdkDeInit();

    return 0;
}
