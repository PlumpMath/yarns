CC=clang
CXX=llvm-g++
ARCH=i386
INCLUDES=-Idata -Iinclude/yarns -Ilib -Isched -Isystem
# DEBUG
#CFLAGS=-O0 -gfull -pipe -Wall -arch $(ARCH) $(INCLUDES)
# RELEASE
CFLAGS=-O4 -DNDEBUG -pipe -arch $(ARCH) $(INCLUDES)
# END
TEST_CFLAGS=-arch $(ARCH) $(INCLUDES)
LDFLAGS=-L. -arch $(ARCH)

OBJECTS=obj/pages.o obj/sched_multilevel.o obj/sched_roundrobin.o obj/yarn.o obj/smp_scheduler.o obj/sched_random.o obj/sched_staircase.o obj/alloc.o obj/rbtree.o obj/sched_rb.o obj/preempt.o obj/queue.o

test: obj/test.o libyarns.a
	$(CXX) $(LDFLAGS) -o $@ $^

# DEBUG
#libyarns.a: $(OBJECTS)
#	ar rcs $@ $^

# RELEASE
libyarns.a: obj/libyarns.o
	ar rcs $@ $^

obj/libyarns.o: obj/libyarns.s
	as -arch $(ARCH) -o $@ $<

obj/libyarns.s: obj/libyarns.bc
	llc -f -o $@ $<

obj/libyarns.bc: $(OBJECTS)
	llvm-ld -internalize-public-api-file=exports.txt -link-as-library -o $@ $^
# END

obj/test.o: test.c lib/yarn.h system/alloc.h lib/config.h
	$(CC) $(TEST_CFLAGS) -c -o $@ $<

obj/pages.o: system/pages.c system/pages.h lib/config.h lib/debug.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_multilevel.o: sched/sched_multilevel.c lib/config.h sched/scheduler.h lib/debug.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_roundrobin.o: sched/sched_roundrobin.c lib/config.h sched/scheduler.h lib/debug.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_random.o: sched/sched_random.c lib/config.h sched/scheduler.h lib/debug.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_rb.o: sched/sched_rb.c sched/scheduler.h system/alloc.h lib/debug.h data/rbtree.h lib/config.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_staircase.o: sched/sched_staircase.c sched/scheduler.h system/alloc.h data/queue.h lib/debug.h lib/config.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/yarn.o: lib/yarn.c lib/yarn.h sched/smp_scheduler.h lib/config.h sched/scheduler.h system/pages.h system/alloc.h lib/debug.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/smp_scheduler.o: sched/smp_scheduler.c sched/smp_scheduler.h sched/scheduler.h lib/config.h system/lock.h system/atomic.h lib/debug.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/alloc.o: system/alloc.c system/alloc.h system/pages.h lib/debug.h lib/config.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/rbtree.o: data/rbtree.cpp data/rbtree.h system/alloc.h
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/preempt.o: system/preempt.c system/preempt.h data/rbtree.h system/lock.h system/atomic.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/queue.o: data/queue.c data/queue.h system/alloc.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f obj/*.o obj/*.s obj/*.bc test libyarns.a
