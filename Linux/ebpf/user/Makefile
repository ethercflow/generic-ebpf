BASE=	../../..
include $(BASE)/Makefile.inc

SRC_DIR:=	$(BASE)/sys/dev/ebpf

ebpf-objs+=	$(SRC_DIR)/ebpf_allocator.o
ebpf-objs+=	$(SRC_DIR)/ebpf_map.o
ebpf-objs+=	$(SRC_DIR)/ebpf_map_bad.o
ebpf-objs+=	$(SRC_DIR)/ebpf_map_array.o
ebpf-objs+=	$(SRC_DIR)/ebpf_map_hashtable.o
ebpf-objs+=	$(SRC_DIR)/ebpf_prog.o
ebpf-objs+=	$(SRC_DIR)/ebpf_prog_bad.o
ebpf-objs+=	$(SRC_DIR)/ebpf_prog_test.o
ebpf-objs+=	$(SRC_DIR)/ebpf_verifer.o
ebpf-objs+=	$(SRC_DIR)/ebpf_vm.o
ebpf-objs+=	$(SRC_DIR)/ebpf_epoch.o
ebpf-objs+=	./ebpf_linux_user.o

ifeq ($(shell arch), x86_64)
ebpf-objs+=	$(BASE)/sys/amd64/amd64/ebpf_jit_x86_64.o
endif

OBJS=	$(ebpf-objs) $(CKOBJS)

CFLAGS += \
	-Ofast \
	-fPIC \
	-I $(SRC_DIR) \
	-I $(BASE)/sys \
	-I $(BASE) \
	-I $(CKPATH)/include \
	-I $(CURDIR) \
	-Wno-declaration-after-statement \
	-std=gnu99
LIBS = -lpthread

ifeq ($(COVERAGE), "1")
CFLAGS+=	-coverage
LDFLAGS+=	-coverage
endif

all: libebpf.so

libebpf.so: $(CKOBJS) $(ebpf-objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $(OBJS) ${LIBS}

install:
	cp libebpf.so $(DESTDIR)

CLEANFILES=	libebpf.so $(OBJS)
