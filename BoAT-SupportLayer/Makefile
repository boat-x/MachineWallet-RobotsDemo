# Select one of the crypto-dep and platform
ifneq (,$(BOAT_LOG_LEVEL))
BOAT_CFLAGS +=  -DBOAT_LOG_LEVEL=$(BOAT_LOG_LEVEL)
endif

.PHONY:all common crypto_default keccak  keystore platform keypair   third-party tests

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)

    BUILD_DEP := common third-party  keypair   crypto_default keystore  platform 

else

    BUILD_DEP :=

endif

all: $(BUILD_DEP) 

test: $(BUILD_DEP) tests

third-party:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party all

tests:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/tests all

keypair:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keypair all

mbedTLS:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto/mbedTLS all

crypto_default:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto all

keccak:	
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto/keccak all

common:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common all

	
platform:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform all

keystore:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore all
	
clean:
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keypair clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party/crypto clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/common clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/platform clean 
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/keystore clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/tests clean
	make -C $(BOAT_BASE_DIR)/BoAT-SupportLayer/third-party clean


