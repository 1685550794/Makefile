objs := client.o text.o

client:$(objs)
	gcc -o client $^

# .a.o.d .b.o.d
dep_files := $(foreach f,$(objs),.$(f).d)
dep_files := $(wildcard $(dep_files))

ifneq ($(dep_files),)
  include $(dep_files)
endif

%.o : %.c 
	gcc -Wp,-MD,.$@.d -c -o $@ $<

clean:
	rm *.o client

