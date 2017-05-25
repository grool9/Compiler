# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl -ly

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
test:
	./parser ../Test/a-10 ../Test/out.ir
testall:
	./parser ../Test/test2-1.cmm ../Test/out2-1.ir
	./parser ../Test/test2-2.cmm ../Test/out2-2.ir
	./parser ../Test/test2-3.cmm ../Test/out2-3.ir
	./parser ../Test/test2-4.cmm ../Test/out2-4.ir
	./parser ../Test/test1-5.cmm ../Test/out5.ir
testa:
	./parser ../Test/a-1 ../Test/a1_out.ir
	./parser ../Test/a-2 ../Test/a2_out.ir
	./parser ../Test/a-3 ../Test/a3_out.ir
	./parser ../Test/a-4 ../Test/a4_out.ir
	./parser ../Test/a-5 ../Test/a5_out.ir
	./parser ../Test/a-6 ../Test/a6_out.ir
	./parser ../Test/a-7 ../Test/a7_out.ir
	./parser ../Test/a-8 ../Test/a8_out.ir
	./parser ../Test/a-9 ../Test/a9_out.ir
	./parser ../Test/a-10 ../Test/a10_out.ir
	./parser ../Test/a-11 ../Test/a11_out.ir
	./parser ../Test/a-12 ../Test/a12_out.ir
	./parser ../Test/a-13 ../Test/a13_out.ir
testb:
	./parser ../Test/b1.c ../Test/b1_out.ir
	./parser ../Test/b2.c ../Test/b2_out.ir
testc:
	./parser ../Test/c1.c ../Test/c1_out.ir
	./parser ../Test/c2.c ../Test/c2_out.ir
testd:
	./parser ../Test/d1.c ../Test/d1_out.ir
	./parser ../Test/d2.c ../Test/d2_out.ir
	./parser ../Test/d3.c ../Test/d3_out.ir	
teste:
	./parser ../Test/e3.c ../Test/e3_out.ir
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
