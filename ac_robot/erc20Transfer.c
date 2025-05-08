/******************************************************************************
This file is generated from contract ABI. DO NOT modify it by hand.
******************************************************************************/


// Generated C function interface from smart contract ABI

#include "boatconfig.h"
#include "boatiotsdk.h"
#include "boatEngine.h"
#include "boatosal.h"
#include "boat_keystore_intf.h"
#include "api_ethereum.h"


// Types specially defined for mapping from Solidity

    typedef BUINT8 Bbytes1[1];
    typedef BUINT8 Bbytes2[2];
    typedef BUINT8 Bbytes3[3];
    typedef BUINT8 Bbytes4[4];
    typedef BUINT8 Bbytes5[5];
    typedef BUINT8 Bbytes6[6];
    typedef BUINT8 Bbytes7[7];
    typedef BUINT8 Bbytes8[8];

    typedef BUINT8 Bbytes9[9];
    typedef BUINT8 Bbytes10[10];
    typedef BUINT8 Bbytes11[11];
    typedef BUINT8 Bbytes12[12];
    typedef BUINT8 Bbytes13[13];
    typedef BUINT8 Bbytes14[14];
    typedef BUINT8 Bbytes15[15];
    typedef BUINT8 Bbytes16[16];

    typedef BUINT8 Bbytes17[17];
    typedef BUINT8 Bbytes18[18];
    typedef BUINT8 Bbytes19[19];
    typedef BUINT8 Bbytes20[20];
    typedef BUINT8 Bbytes21[21];
    typedef BUINT8 Bbytes22[22];
    typedef BUINT8 Bbytes23[23];
    typedef BUINT8 Bbytes24[24];

    typedef BUINT8 Bbytes25[25];
    typedef BUINT8 Bbytes26[26];
    typedef BUINT8 Bbytes27[27];
    typedef BUINT8 Bbytes28[28];
    typedef BUINT8 Bbytes29[29];
    typedef BUINT8 Bbytes30[30];
    typedef BUINT8 Bbytes31[31];
    typedef BUINT8 Bbytes32[32];

    typedef Bbytes16 BUINT128;
    typedef Bbytes16 BSINT128;
    typedef Bbytes32 BUINT256;
    typedef Bbytes32 BSINT256;

BCHAR *erc20Transfer_name(BoatEthTx *tx_ptr)
{
    BCHAR *call_result_str = NULL;
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if (tx_ptr == NULL)
    {
        return NULL;
    }

    data_field.field_len = (0 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "name()";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    call_result_str = BoatEthCallContractFunc(tx_ptr, function_prototye_str, data_field.field_ptr+4, data_field.field_len-4);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(call_result_str);

}

BCHAR *erc20Transfer_symbol(BoatEthTx *tx_ptr)
{
    BCHAR *call_result_str = NULL;
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if (tx_ptr == NULL)
    {
        return NULL;
    }

    data_field.field_len = (0 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "symbol()";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    call_result_str = BoatEthCallContractFunc(tx_ptr, function_prototye_str, data_field.field_ptr+4, data_field.field_len-4);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(call_result_str);

}

BCHAR *erc20Transfer_decimals(BoatEthTx *tx_ptr)
{
    BCHAR *call_result_str = NULL;
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if (tx_ptr == NULL)
    {
        return NULL;
    }

    data_field.field_len = (0 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "decimals()";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    call_result_str = BoatEthCallContractFunc(tx_ptr, function_prototye_str, data_field.field_ptr+4, data_field.field_len-4);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(call_result_str);

}

BCHAR *erc20Transfer_totalSupply(BoatEthTx *tx_ptr)
{
    BCHAR *call_result_str = NULL;
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if (tx_ptr == NULL)
    {
        return NULL;
    }

    data_field.field_len = (0 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "totalSupply()";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    call_result_str = BoatEthCallContractFunc(tx_ptr, function_prototye_str, data_field.field_ptr+4, data_field.field_len-4);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(call_result_str);

}

BCHAR *erc20Transfer_balanceOf(BoatEthTx *tx_ptr, BoatAddress _owner)
{
    BCHAR *call_result_str = NULL;
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (_owner == NULL))
    {
        return NULL;
    }

    data_field.field_len = (1 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "balanceOf(address)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_owner, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), _owner, sizeof(BoatAddress));
    data_offset_ptr += 32;
    call_result_str = BoatEthCallContractFunc(tx_ptr, function_prototye_str, data_field.field_ptr+4, data_field.field_len-4);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(call_result_str);

}

BCHAR *erc20Transfer_transfer(BoatEthTx *tx_ptr, BoatAddress _to, BUINT256 _value)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (_to == NULL)|| (_value == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (2 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "transfer(address,uint256)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_to, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), _to, sizeof(BoatAddress));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_value, sizeof(BUINT256));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT256), _value, sizeof(BUINT256));
    data_offset_ptr += 32;
    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *erc20Transfer_allowance(BoatEthTx *tx_ptr, BoatAddress _owner, BoatAddress _spender)
{
    BCHAR *call_result_str = NULL;
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (_owner == NULL)|| (_spender == NULL))
    {
        return NULL;
    }

    data_field.field_len = (2 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "allowance(address,address)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_owner, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), _owner, sizeof(BoatAddress));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_spender, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), _spender, sizeof(BoatAddress));
    data_offset_ptr += 32;
    call_result_str = BoatEthCallContractFunc(tx_ptr, function_prototye_str, data_field.field_ptr+4, data_field.field_len-4);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(call_result_str);

}

BCHAR *erc20Transfer_approve(BoatEthTx *tx_ptr, BoatAddress _spender, BUINT256 _value)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (_spender == NULL)|| (_value == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (2 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "approve(address,uint256)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_spender, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), _spender, sizeof(BoatAddress));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_value, sizeof(BUINT256));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT256), _value, sizeof(BUINT256));
    data_offset_ptr += 32;
    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

BCHAR *erc20Transfer_transferFrom(BoatEthTx *tx_ptr, BoatAddress _from, BoatAddress _to, BUINT256 _value)
{
    static BCHAR tx_hash_str[67] = "";
    BoatFieldVariable data_field = {NULL, 0};
    BCHAR *function_prototye_str;
    BUINT8 field_bytes32[32];
    BUINT8 fixedsize_bytes32[32];
    BUINT8 *data_offset_ptr;
    BUINT32 i;
    (void) fixedsize_bytes32;
    (void) i;
    boat_try_declare;

    if ((tx_ptr == NULL) || (_from == NULL)|| (_to == NULL)|| (_value == NULL))
    {
        return NULL;
    }

    boat_try(BoatEthTxSetNonce(tx_ptr, BOAT_ETH_NONCE_AUTO));

    data_field.field_len = (3 * 32 + 4);
    data_field.field_ptr = BoatMalloc(data_field.field_len);
    if (data_field.field_ptr == NULL)
    {
        boat_throw(BOAT_ERROR, cleanup);
    }
    data_offset_ptr = data_field.field_ptr;

    function_prototye_str = "transferFrom(address,address,uint256)";
    BoatHash(BOAT_HASH_KECCAK256, (BUINT8 *)function_prototye_str, strlen(function_prototye_str), field_bytes32, NULL, NULL);
    memcpy(data_offset_ptr, field_bytes32, 4);
    data_offset_ptr += 4;

    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_from, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), _from, sizeof(BoatAddress));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_to, sizeof(BoatAddress));
    memcpy(data_offset_ptr + 32 - sizeof(BoatAddress), _to, sizeof(BoatAddress));
    data_offset_ptr += 32;
    memset(data_offset_ptr, 0x00, 32);
    UtilityChangeEndian(_value, sizeof(BUINT256));
    memcpy(data_offset_ptr + 32 - sizeof(BUINT256), _value, sizeof(BUINT256));
    data_offset_ptr += 32;
    boat_try(BoatEthTxSetData(tx_ptr, &data_field));

    boat_try(BoatEthTxSend(tx_ptr));

    UtilityBinToHex(tx_hash_str, tx_ptr->tx_hash.field, tx_ptr->tx_hash.field_len, BIN2HEX_LEFTTRIM_UNFMTDATA, BIN2HEX_PREFIX_0x_YES, BOAT_FALSE);


    boat_catch(cleanup)
    {
        if (data_field.field_ptr != NULL)
        {
            BoatFree(data_field.field_ptr);
        }
        return(NULL);
    }
        
    BoatFree(data_field.field_ptr);
    return(tx_hash_str);

}

