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

#ifndef SOLIDITY_TYPE_MAPPING 
#define SOLIDITY_TYPE_MAPPING 

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

#endif 
BCHAR *erc20Transfer_name(BoatEthTx *tx_ptr);
BCHAR *erc20Transfer_symbol(BoatEthTx *tx_ptr);
BCHAR *erc20Transfer_decimals(BoatEthTx *tx_ptr);
BCHAR *erc20Transfer_totalSupply(BoatEthTx *tx_ptr);
BCHAR *erc20Transfer_balanceOf(BoatEthTx *tx_ptr, BoatAddress _owner);
BCHAR *erc20Transfer_transfer(BoatEthTx *tx_ptr, BoatAddress _to, BUINT256 _value);
BCHAR *erc20Transfer_allowance(BoatEthTx *tx_ptr, BoatAddress _owner, BoatAddress _spender);
BCHAR *erc20Transfer_approve(BoatEthTx *tx_ptr, BoatAddress _spender, BUINT256 _value);
BCHAR *erc20Transfer_transferFrom(BoatEthTx *tx_ptr, BoatAddress _from, BoatAddress _to, BUINT256 _value);
