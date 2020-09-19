#diretorios de saida
ARM_OUT_DIR			= $(BASE_OUT_DIR)/$(PROJECT)/arm/bin
WIN_OUT_DIR			= $(BASE_OUT_DIR)/$(PROJECT)/win/bin
LIN_OUT_DIR			= $(BASE_OUT_DIR)/$(PROJECT)/lin/bin
 
#nome do binario gerado
BINARY_NAME			= yadma

ifeq ($(BUILD_TYPE),static_lib)
BINARY_EXT			= .a
endif

ifeq ($(BUILD_TYPE),shared_lib)
BINARY_EXT			= .so
endif

ifeq ($(type),windows)

ifeq ($(BUILD_TYPE),executable)
BINARY_EXT			= .exe
endif

ifeq ($(BUILD_TYPE),shared_lib)
BINARY_EXT			= .dll
endif

endif

#Bibliotecas

LIBS_STATIC			= -lstdc++ -lcurl -lssl -lcrypto -lrt -lsqlite3 -lmongoose 
LIBS_DYNAMIC		= -lpthread -lc -ldl 
LIBS_ARM_STATIC  	= -lz 
LIBS_LIN_STATIC		= 
LIBS_WIN_STATIC		=
LIBS_ARM_DYNAMIC	= 
LIBS_LIN_DYNAMIC	=
LIBS_WIN_DYNAMIC	=

#diretorios das bibliotecas e include das mesmas
LIBDIR		    ?= ../../../res/libs

#includes
INCLUDES_DIRS	    = -I$(LIBDIR)/curl/include -I$(LIBDIR)/openssl/include -I$(LIBDIR)/sqlite/include -I$(LIBDIR)/unittest/include -I$(LIBDIR)/smart/include -I$(LIBDIR)/mongoose/include
#libs
ARM_LIBRARY_DIRS    = -L$(LIBDIR)/curl/arm -L$(LIBDIR)/openssl/arm -L$(LIBDIR)/sqlite/arm -L$(LIBDIR)/unittest/arm -L$(LIBDIR)/mongoose/arm

LIN_LIBRARY_DIRS    = -L$(LIBDIR)/curl/linux -L$(LIBDIR)/openssl/linux -L$(LIBDIR)/sqlite/linux -L$(LIBDIR)/unittest/linux -L$(LIBDIR)/mongoose/linux

WIN_LIBRARY_DIRS    = -L$(LIBDIR)/curl/windows -L$(LIBDIR)/openssl/windows

#diretorios dos compiladores para plataformas windows, linux e arm-linux
ARM_BIN_PATH		?= ../../../../snakeos-sdk/tools/arm-uclibc-3.4.6/bin
WIN_BIN_PATH		?= /usr/bin
LIN_BIN_PATH		?= /usr/bin

#nome dos compiladores
ARM_GCC 			= $(ARM_BIN_PATH)/arm-linux-gcc
WIN_GCC 			= $(WIN_BIN_PATH)/gcc
LIN_GCC 			= $(LIN_BIN_PATH)/gcc

#retirar informacoes de debugs (binario +leve)
ARM_STRIP			= $(ARM_BIN_PATH)/arm-linux-strip -s --strip-all
LIN_STRIP			= $(LIN_BIN_PATH)/strip -s --strip-all
WIN_STRIP			= echo windows com simbolos

#flags de compilação
ARM_CXXFLAGS		= -Wall -O	
WIN_CXXFLAGS		= -g -Wall -O 
LIN_CXXFLAGS		= -g -Wall -O

#flags de "linkagem"
ARM_LDFLAGS	    	= 
LIN_LDFLAGS	    	= -g
WIN_LDFLAGS	    	= -g


##################################################################################################################################
##################################################################################################################################
##################################################################################################################################
#############################	Abaixo teoricamente nao há o que mexer para adaptar o make file ##################################
##################################################################################################################################
##################################################################################################################################

#arquivos
BASE_DIR			= 
#bibliotecas utilizadas (no inicio da linha é estatico, apos -Bdynamic é de linkagem dinamica
#libc deve ser dinamica por causa de socket.h em ambientes unix (vai dar pau no arm se nao fizer isso resolucao de nomes nao funfa)
WIN_LIBS			= -Wl,-Bstatic $(LIBS_STATIC) $(LIBS_WIN_STATIC) -Wl,-Bdynamic $(LIBS_DYNAMIC) $(LIBS_WIN_DYNAMIC)
ARM_LIBS			= -Wl,-Bstatic $(LIBS_STATIC) $(LIBS_ARM_STATIC) -Wl,-Bdynamic $(LIBS_DYNAMIC) $(LIBS_ARM_DYNAMIC)
LIN_LIBS			= -Wl,-Bstatic $(LIBS_STATIC) $(LIBS.linSTATIC) -Wl,-Bdynamic $(LIBS_DYNAMIC) $(LIBS_LIN_DYNAMIC)

#colocar arquivos objetos no diretorio de saida dos binarios
ifeq ($(type),linux)
BASE_DIR  			= obj/lin		
BASE_SUFIX			= .lin
DEFINES 			= -D_LINUX -D_NARM
endif

ifeq ($(type),windows)
BASE_DIR  			= obj/win
BASE_SUFIX			= _win_		
DEFINES 			= -D_WINDOWS -D_NARM
endif

ifeq ($(type),arm)
BASE_DIR  			= obj/arm 
BASE_SUFIX			= _arm_		
DEFINES 			= -D_LINUX -D_ARM
endif

ifeq ($(debug),1)
DEFINES 			+= -D_DEBUG
endif

#redefinir nomes dos binario caso seja testes .exe no windows e sem extensao no linux
ifneq ($(type),arm)

ifeq ($(unittest),1)
BUILD_TYPE			= executable
DEFINES 			+= -D_UNIT_TEST
RUN_TESTS 			= 1
BINARY_NAME 		= test_$(BINARY_NAME)

ifeq ($(type),windows)
BINARY_EXT = .exe
else
BINARY_EXT =
endif

endif
endif

#caminho de saida para o binário
ARM_OUT_PATH		= $(ARM_OUT_DIR)/$(BINARY_NAME)$(BINARY_EXT)
WIN_OUT_PATH		= $(WIN_OUT_DIR)/$(BINARY_NAME)$(BINARY_EXT)
LIN_OUT_PATH		= $(LIN_OUT_DIR)/$(BINARY_NAME)$(BINARY_EXT)

#mudar tipo de "linkagem" caso for lib
ifeq ($(BUILD_TYPE),shared_lib)
ARM_LDFLAGS	    	+= -shared -Wl,-soname,$(BINARY_NAME)$(BINARY_EXT)
LIN_LDFLAGS	    	+= -shared -Wl,-soname,$(BINARY_NAME)$(BINARY_EXT)
WIN_LDFLAGS	    	+= -shared -Wl,-soname,$(BINARY_NAME)$(BINARY_EXT)
endif

#mudar tipo de "linkagem" caso for lib
ifeq ($(BUILD_TYPE),static_lib)
ARM_LDFLAGS	    	+= -static 
LIN_LDFLAGS	    	+= -static
WIN_LDFLAGS	    	+= -static
endif

#_FILES				= $(notdir $(wildcard $(CURDIR)/*.cpp))

_SUBDIRS 			:= $(shell find . -type d -print)
SOURCES				:= $(foreach ITEM,$(_SUBDIRS),$(wildcard $(ITEM)/*.cpp))
SOURCES				:= $(foreach ITEM,$(SOURCES),$(if $(findstring /.c,$(ITEM)),,$(ITEM)))
OBJECTS				:= $(foreach ITEM,$(SOURCES),$(patsubst %.cpp,%.o,$(ITEM)))

#retirar da variavel os objs que não sao da distro (que estao em algun diretorio /win32/, /linux/)
ifeq ($(type),windows)
OBJECTS				:= $(foreach ITEM,$(OBJECTS),$(if $(findstring /linux/,$(ITEM)),,$(ITEM)))
else
OBJECTS				:= $(foreach ITEM,$(OBJECTS),$(if $(findstring /win32/,$(ITEM)),,$(ITEM)))
endif

ifeq ($(clean),1)
CLEAR_ACTION		:= clean_$(type)
endif

all: create_object_dir $(if $(CLEAR_ACTION), $(CLEAR_ACTION), $(if $(CLEAR_ACTION), NONE, GEN_BIN ) ) 

#--------------------------------------------------------------------------------------------
create_object_dir: 
ifeq ($(type),lin)
	@mkdir -p $(LIN_OUT_DIR)
	@mkdir -p $(CURDIR)/obj/lin
endif
ifeq ($(type),win)
	@mkdir -p $(WIN_OUT_DIR)
	@mkdir -p $(CURDIR)/obj/win
endif
ifeq ($(type),arm)
	@mkdir -p $(ARM_OUT_DIR)
	@mkdir -p $(CURDIR)/obj/arm
endif

#--------------------------------------------------------------------------------------------
%.o: %.cpp
	
ifeq ($(type),linux)
	@echo compilando $< 
	$(LIN_GCC) -c $(INCLUDES_DIRS) -o ./obj/lin/$(notdir $@) $< $(LIN_CXXFLAGS) $(DEFINES)
endif
ifeq ($(type),arm)
	@echo compilando $< 
	$(ARM_GCC) -c $(INCLUDES_DIRS) -o ./obj/arm/$(notdir $@) $< $(ARM_CXXFLAGS) $(DEFINES)
endif
ifeq ($(type),windows)
	@echo compilando $< 
	$(WIN_GCC) -c $(INCLUDES_DIRS) -o ./obj/arm/$(notdir $@) $< $(WIN_CXXFLAGS) $(DEFINES)
endif

#--------------------------------------------------------------------------------------------	
clean_linux:
	@echo
	@echo limpando $(LIN_OUT_DIR)
	@$(RM) $(LIN_OUT_PATH) $(foreach item,$(notdir $(OBJECTS)),./obj/lin/$(item))
	@echo
	
#--------------------------------------------------------------------------------------------	
clean_arm:
	@echo
	@echo limpando $(ARM_OUT_DIR)
	@$(RM) $(ARM_OUT_PATH) $(foreach item,$(notdir $(OBJECTS)),./obj/arm/$(item))
	@echo
	
#--------------------------------------------------------------------------------------------	
clean_windows:
	@echo
	@echo limpando $(WIN_OUT_DIR)
	@$(RM) $(WIN_OUT_PATH) $(foreach item,$(notdir $(OBJECTS)),./obj/win/$(item))
	@echo

#--------------------------------------------------------------------------------------------	
NONE:
	@echo Limpo
#--------------------------------------------------------------------------------------------	
GEN_BIN:  GEN_BIN_$(type)

#--------------------------------------------------------------------------------------------
GEN_BIN_linux: $(OBJECTS)
	@echo ...
	@echo gerando binario $(BINARY_NAME)$(BINARY_EXT) em $(LIN_OUT_DIR)
	@$(LIN_GCC) $(foreach item,$(notdir $(OBJECTS)),./obj/lin/$(item)) $(LIN_LDFLAGS) $(LIN_LIBS) $(LIN_LIBRARY_DIRS) -o $(LIN_OUT_PATH)
	@echo

ifeq ($(RUN_TESTS),1)
	@echo executar testes
	@$(LIN_OUT_PATH)
	@echo
endif

ifneq ($(debug),1)
	@echo retirando simbolos
	$(LIN_STRIP) $(LIN_OUT_PATH)
	@echo
endif

#--------------------------------------------------------------------------------------------
GEN_BIN_windows: $(OBJECTS)
	@echo ...
	@echo gerando binario $(BINARY_NAME)$(BINARY_EXT) em $(WIN_OUT_DIR)
	@$(WIN_GCC) $(foreach item,$(notdir $(OBJECTS)),./obj/win/$(item)) $(WIN_LDFLAGS) $(WIN_LIBS) $(WIN_LIBRARY_DIRS) -o $(WIN_OUT_PATH)
	@echo
ifeq ($(RUN_TESTS),1)
	@echo executar testes
	$(WIN_OUT_PATH)
	@echo
endif

#--------------------------------------------------------------------------------------------
GEN_BIN_arm: $(OBJECTS)
	@echo ...
	@echo gerando binario $(BINARY_NAME)$(BINARY_EXT) em $(ARM_OUT_DIR)
	@$(ARM_GCC) $(foreach item,$(notdir $(OBJECTS)),./obj/arm/$(item)) $(ARM_LDFLAGS) $(ARM_LIBS) $(ARM_LIBRARY_DIRS) -o $(ARM_OUT_PATH)
	@echo
ifneq ($(debug),1)
#	@echo retirando simbolos
#	$(ARM_STRIP) $(ARM_OUT_PATH)
#	@echo
endif


