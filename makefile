all: kmc generate_uniq change_format

KMC_BIN_DIR = bin
KMC_MAIN_DIR = kmer_counter
UNIQ_MAIN_DIR = find_unique
CHANGE_FORMAT = change_format

CC 	= g++
CFLAGS	= -Wall -O3 -m64 -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -std=c++11 -g
CLINK	= -lm -static -O3 -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -std=c++11 -g

KMC_OBJS = \
$(KMC_MAIN_DIR)/kmer_counter.o \
$(KMC_MAIN_DIR)/mmer.o \
$(KMC_MAIN_DIR)/mem_disk_file.o \
$(KMC_MAIN_DIR)/rev_byte.o \
$(KMC_MAIN_DIR)/bkb_writer.o \
$(KMC_MAIN_DIR)/cpu_info.o \
$(KMC_MAIN_DIR)/bkb_reader.o \
$(KMC_MAIN_DIR)/fastq_reader.o \
$(KMC_MAIN_DIR)/timer.o \
$(KMC_MAIN_DIR)/develop.o \
$(KMC_MAIN_DIR)/kb_completer.o \
$(KMC_MAIN_DIR)/kb_storer.o \
$(KMC_MAIN_DIR)/kmer.o \
$(KMC_MAIN_DIR)/splitter.o \
$(KMC_MAIN_DIR)/kb_collector.o \
$(KMC_MAIN_DIR)/kff_writer.o

RADULS_OBJS = \
$(KMC_MAIN_DIR)/raduls_sse2.o \
$(KMC_MAIN_DIR)/raduls_sse41.o \
$(KMC_MAIN_DIR)/raduls_avx2.o \
$(KMC_MAIN_DIR)/raduls_avx.o

UNIQ_OBJS = \
$(UNIQ_MAIN_DIR)/unique_kmer.o \
$(UNIQ_MAIN_DIR)/generate_uniq.o

CHANGE_FORMAT_OBJS= \
$(UNIQ_MAIN_DIR)/change_format.o \

KMC_LIBS = \
$(KMC_MAIN_DIR)/libs/libz.a \
$(KMC_MAIN_DIR)/libs/libbz2.a


$(KMC_OBJS) $(CHANGE_FORMAT_OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(UNIQ_OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -DRVS

$(KMC_MAIN_DIR)/raduls_sse2.o: $(KMC_MAIN_DIR)/raduls_sse2.cpp
	$(CC) $(CFLAGS) -msse2 -c $< -o $@
$(KMC_MAIN_DIR)/raduls_sse41.o: $(KMC_MAIN_DIR)/raduls_sse41.cpp
	$(CC) $(CFLAGS) -msse4.1 -c $< -o $@
$(KMC_MAIN_DIR)/raduls_avx.o: $(KMC_MAIN_DIR)/raduls_avx.cpp
	$(CC) $(CFLAGS) -mavx -c $< -o $@
$(KMC_MAIN_DIR)/raduls_avx2.o: $(KMC_MAIN_DIR)/raduls_avx2.cpp
	$(CC) $(CFLAGS) -mavx2 -c $< -o $@

kmc: $(KMC_OBJS) $(RADULS_OBJS)
	-mkdir -p $(KMC_BIN_DIR)
	$(CC) $(CLINK) -o $(KMC_BIN_DIR)/$@ $^ $(KMC_LIBS)

generate_uniq: ${UNIQ_OBJS}
	-mkdir -p $(KMC_BIN_DIR)
	$(CC) $(CLINK) -o $(KMC_BIN_DIR)/$@ $^ $(UNIQ_LIBS) -DRVS

change_format: ${CHANGE_FORMAT_OBJS}
	-mkdir -p $(KMC_BIN_DIR)
	$(CC) $(CLINK) -o $(KMC_BIN_DIR)/$@ $^ 

clean:
	-rm -f $(KMC_MAIN_DIR)/*.o
	-rm -f ${UNIQ_MAIN_DIR}/*.o
	#-rm -rf bin
