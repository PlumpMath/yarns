CC={$cc}
CXX={$cxx}
INCLUDES=-Idata -Iinclude/yarns -Ilib -Isched -Isystem -Icontext/{$context}
CFLAGS={$lib_cflags} $(INCLUDES) -DYARNS_CURRENT_VERSION=\"`git describe`\"
TEST_CFLAGS={$bin_cflags} $(INCLUDES)
LDFLAGS={$ldflags} {if $strip}-exported_symbols_list exports.txt{/if}{if $linux_style_strip}--retain-symbols-file exports.txt{/if}

COBJECTS=obj/pages.o obj/sched_roundrobin.o obj/yarn.o obj/master_scheduler.o obj/sched_staircase.o obj/alloc.o obj/rbtree.o obj/sched_fair.o obj/preempt.o obj/queue.o obj/queue_atomic.o obj/context.o obj/timer.o obj/pool.o obj/wait_graph.o
AOBJECTS={if $context_asm}obj/context_asm.o{/if}

test: obj/test.o libyarns.a
	$(CXX) $(LDFLAGS) -o $@ $^

libyarns.a: obj/ymain.o
	ar rcs $@ $^
	
{ifnot $lto}
obj/ymain.o: $(COBJECTS) $(AOBJECTS)
	ld $(LDFLAGS) -r $^ -o $@
{/ifnot}
{if $lto}
obj/ymain.o: obj/libyarns.o $(AOBJECTS)
	ld $(LDFLAGS) -r $^ -o $@

obj/libyarns.o: obj/libyarns.s
	{$as} {$asflags} -o $@ $<

obj/libyarns.s: obj/libyarns.bc
	llc -f -o $@ $<

obj/libyarns.bc: $(COBJECTS)
	llvm-ld -link-as-library -o $@ $^
{/if}

obj/test.o: test.c include/yarns/yarn.h lib/alloc.h include/yarns/config.h Makefile
	$(CC) $(TEST_CFLAGS) -c -o $@ $<

obj/pages.o: system/pages.c system/pages.h include/yarns/config.h lib/debug.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_roundrobin.o: sched/sched_roundrobin.c include/yarns/config.h sched/scheduler.h lib/debug.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_staircase.o: sched/sched_staircase.c sched/scheduler.h lib/alloc.h data/queue.h lib/debug.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/sched_fair.o: sched/sched_fair.c sched/scheduler.h lib/alloc.h data/rbtree.h lib/debug.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/yarn.o: lib/yarn.c include/yarns/yarn.h sched/master_scheduler.h include/yarns/config.h sched/scheduler.h lib/pool.h lib/yarn-internal.h system/pages.h lib/alloc.h lib/debug.h context/{$context}/context.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/master_scheduler.o: sched/master_scheduler.c sched/master_scheduler.h sched/scheduler.h include/yarns/config.h system/lock.h system/atomic.h lib/debug.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/wait_graph.o: lib/wait_graph.c lib/wait_graph.h lib/timer.h include/yarns/yarn.h lib/yarn-internal.h lib/alloc.h system/pages.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/alloc.o: lib/alloc.c lib/alloc.h system/pages.h lib/debug.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/rbtree.o: data/rbtree.cpp data/rbtree.h lib/alloc.h include/yarns/config.h Makefile
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/preempt.o: system/preempt.c system/preempt.h data/rbtree.h system/lock.h system/atomic.h lib/timer.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/queue.o: data/queue.c data/queue.h lib/alloc.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/queue_atomic.o: data/queue_atomic.c include/yarns/queue_atomic.h lib/alloc.h system/atomic.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/context.o: context/{$context}/context.c context/{$context}/context.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

{if $context_asm}
obj/context_asm.o: context/{$context}/context.s Makefile
	{$as} {$asflags} -o $@ $<
{/if}

obj/timer.o: lib/timer.c lib/timer.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

obj/pool.o: lib/pool.c lib/pool.h lib/alloc.h lib/debug.h include/yarns/config.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f obj/*.o obj/*.s obj/*.bc test libyarns.a
