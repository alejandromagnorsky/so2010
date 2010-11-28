	.file	"klib.c"
	.text
.globl _sys_write
	.type	_sys_write, @function
_sys_write:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 16 "../src/klib.c" 1
	movl $1, %eax;
# 0 "" 2
# 17 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 18 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 19 "../src/klib.c" 1
	movl 16(%ebp), %edx;
# 0 "" 2
# 21 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 23 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_write, .-_sys_write
.globl _sys_read
	.type	_sys_read, @function
_sys_read:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 30 "../src/klib.c" 1
	movl $0, %eax;
# 0 "" 2
# 31 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 32 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 33 "../src/klib.c" 1
	movl 16(%ebp), %edx;
# 0 "" 2
# 35 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 37 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_read, .-_sys_read
.globl _sys_seekr
	.type	_sys_seekr, @function
_sys_seekr:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 44 "../src/klib.c" 1
	movl $2, %eax;
# 0 "" 2
# 45 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 46 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 47 "../src/klib.c" 1
	movl 16(%ebp), %edx;
# 0 "" 2
# 49 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 51 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_seekr, .-_sys_seekr
.globl _sys_seekw
	.type	_sys_seekw, @function
_sys_seekw:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 58 "../src/klib.c" 1
	movl $3, %eax;
# 0 "" 2
# 59 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 60 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 61 "../src/klib.c" 1
	movl 16(%ebp), %edx;
# 0 "" 2
# 63 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 65 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_seekw, .-_sys_seekw
.globl _sys_tellr
	.type	_sys_tellr, @function
_sys_tellr:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 72 "../src/klib.c" 1
	movl $4, %eax;
# 0 "" 2
# 73 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 75 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 77 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_tellr, .-_sys_tellr
.globl _sys_tellw
	.type	_sys_tellw, @function
_sys_tellw:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 84 "../src/klib.c" 1
	movl $5, %eax;
# 0 "" 2
# 85 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 87 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 89 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_tellw, .-_sys_tellw
.globl _sys_exec
	.type	_sys_exec, @function
_sys_exec:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 96 "../src/klib.c" 1
	movl $6, %eax;
# 0 "" 2
# 97 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 98 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 100 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 102 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_exec, .-_sys_exec
.globl _sys_exec_b
	.type	_sys_exec_b, @function
_sys_exec_b:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 109 "../src/klib.c" 1
	movl $7, %eax;
# 0 "" 2
# 110 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 111 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 113 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 115 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_exec_b, .-_sys_exec_b
.globl _sys_gettid
	.type	_sys_gettid, @function
_sys_gettid:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 122 "../src/klib.c" 1
	movl $8, %eax;
# 0 "" 2
# 123 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 124 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 125 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_gettid, .-_sys_gettid
.globl _sys_nexttid
	.type	_sys_nexttid, @function
_sys_nexttid:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 133 "../src/klib.c" 1
	movl $9, %eax;
# 0 "" 2
# 134 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 135 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 136 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_nexttid, .-_sys_nexttid
.globl _sys_getrank
	.type	_sys_getrank, @function
_sys_getrank:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 144 "../src/klib.c" 1
	movl $10, %eax;
# 0 "" 2
# 145 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 146 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 147 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_getrank, .-_sys_getrank
.globl _sys_getprio
	.type	_sys_getprio, @function
_sys_getprio:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 155 "../src/klib.c" 1
	movl $11, %eax;
# 0 "" 2
# 156 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 157 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 158 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_getprio, .-_sys_getprio
.globl _sys_getcpuc
	.type	_sys_getcpuc, @function
_sys_getcpuc:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 166 "../src/klib.c" 1
	movl $12, %eax;
# 0 "" 2
# 167 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 169 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 170 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_getcpuc, .-_sys_getcpuc
.globl _sys_name
	.type	_sys_name, @function
_sys_name:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 178 "../src/klib.c" 1
	movl $13, %eax;
# 0 "" 2
# 179 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 181 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 182 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_name, .-_sys_name
.globl _sys_getName
	.type	_sys_getName, @function
_sys_getName:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 190 "../src/klib.c" 1
	movl $14, %eax;
# 0 "" 2
# 191 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 192 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 194 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 195 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_getName, .-_sys_getName
.globl _sys_sleep
	.type	_sys_sleep, @function
_sys_sleep:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 202 "../src/klib.c" 1
	movl $15, %eax;
# 0 "" 2
# 203 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 205 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 206 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_sleep, .-_sys_sleep
.globl _sys_send
	.type	_sys_send, @function
_sys_send:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 214 "../src/klib.c" 1
	movl $17, %eax;
# 0 "" 2
# 215 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 216 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 217 "../src/klib.c" 1
	movl 16(%ebp), %edx;
# 0 "" 2
# 219 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 221 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_send, .-_sys_send
.globl _sys_recv
	.type	_sys_recv, @function
_sys_recv:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
#APP
# 229 "../src/klib.c" 1
	movl $16, %eax;
# 0 "" 2
# 231 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 233 "../src/klib.c" 1
	movl %eax, -4(%ebp);
# 0 "" 2
#NO_APP
	movl	-4(%ebp), %eax
	leave
	ret
	.size	_sys_recv, .-_sys_recv
.globl _sys_getmsg
	.type	_sys_getmsg, @function
_sys_getmsg:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 241 "../src/klib.c" 1
	movl $18, %eax;
# 0 "" 2
# 242 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 243 "../src/klib.c" 1
	movl 12(%ebp), %ecx;
# 0 "" 2
# 245 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 247 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_getmsg, .-_sys_getmsg
.globl _sys_clsmsg
	.type	_sys_clsmsg, @function
_sys_clsmsg:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
#APP
# 255 "../src/klib.c" 1
	movl $19, %eax;
# 0 "" 2
# 257 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 259 "../src/klib.c" 1
	movl %eax, -4(%ebp);
# 0 "" 2
#NO_APP
	movl	-4(%ebp), %eax
	leave
	ret
	.size	_sys_clsmsg, .-_sys_clsmsg
.globl _sys_yield
	.type	_sys_yield, @function
_sys_yield:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
#APP
# 265 "../src/klib.c" 1
	movl $20, %eax;
# 0 "" 2
# 267 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 269 "../src/klib.c" 1
	movl %eax, -4(%ebp);
# 0 "" 2
#NO_APP
	movl	-4(%ebp), %eax
	leave
	ret
	.size	_sys_yield, .-_sys_yield
.globl _sys_kill
	.type	_sys_kill, @function
_sys_kill:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 275 "../src/klib.c" 1
	movl $21, %eax;
# 0 "" 2
# 276 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 278 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 280 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_kill, .-_sys_kill
.globl _sys_setPrio
	.type	_sys_setPrio, @function
_sys_setPrio:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 286 "../src/klib.c" 1
	movl $22, %eax;
# 0 "" 2
# 287 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 289 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 291 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_setPrio, .-_sys_setPrio
.globl _sys_setRank
	.type	_sys_setRank, @function
_sys_setRank:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 297 "../src/klib.c" 1
	movl $23, %eax;
# 0 "" 2
# 298 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 300 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 302 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_setRank, .-_sys_setRank
.globl _sys_getRMode
	.type	_sys_getRMode, @function
_sys_getRMode:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 309 "../src/klib.c" 1
	movl $25, %eax;
# 0 "" 2
# 310 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 311 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 312 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_getRMode, .-_sys_getRMode
.globl _sys_getStatus
	.type	_sys_getStatus, @function
_sys_getStatus:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 320 "../src/klib.c" 1
	movl $26, %eax;
# 0 "" 2
# 321 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 322 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 323 "../src/klib.c" 1
	movl %eax, -8(%ebp);
# 0 "" 2
#NO_APP
	movl	-8(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_getStatus, .-_sys_getStatus
.globl _sys_wait
	.type	_sys_wait, @function
_sys_wait:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
#APP
# 331 "../src/klib.c" 1
	movl $24, %eax;
# 0 "" 2
# 333 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
# 335 "../src/klib.c" 1
	movl %eax, -4(%ebp);
# 0 "" 2
#NO_APP
	movl	-4(%ebp), %eax
	leave
	ret
	.size	_sys_wait, .-_sys_wait
.globl _sys_read_disk
	.type	_sys_read_disk, @function
_sys_read_disk:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 342 "../src/klib.c" 1
	movl $28, %eax;
# 0 "" 2
# 343 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 345 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
#NO_APP
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_read_disk, .-_sys_read_disk
.globl _sys_write_disk
	.type	_sys_write_disk, @function
_sys_write_disk:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
#APP
# 352 "../src/klib.c" 1
	movl $27, %eax;
# 0 "" 2
# 353 "../src/klib.c" 1
	movl 8(%ebp), %ebx;
# 0 "" 2
# 355 "../src/klib.c" 1
	int $0x80;
# 0 "" 2
#NO_APP
	addl	$16, %esp
	popl	%ebx
	leave
	ret
	.size	_sys_write_disk, .-_sys_write_disk
.globl Task
	.data
	.align 32
	.type	Task, @object
	.size	Task, 124
Task:
	.long	_task_setPriority
	.long	_task_setRank
	.long	_task_setStatus
	.long	_task_getPriority
	.long	_task_getRank
	.long	_task_getStatus
	.long	_task_getTID
	.long	_task_findSlot
	.long	_task_new
	.long	_task_kill
	.long	_task_getByTID
	.long	_task_getCurrent
	.long	_task_getByName
	.long	_task_getNewTID
	.long	_task_setupScheduler
	.long	_task_scheduler
	.long	_task_cleaner
	.long	_task_setTty
	.long	_task_getTty
	.long	_task_runInBackground
	.long	_task_getRunningMode
	.long	_task_setParentTID
	.long	_task_getParentTID
	.long	_task_yield
	.long	_task_checkTTY
	.long	_task_setSleep
	.long	_task_decSleep
	.long	_task_getSleep
	.long	_task_maintenance
	.long	_task_setSend
	.long	_task_setRecv
.globl Top
	.align 32
	.type	Top, @object
	.size	Top, 32
Top:
	.long	_top_increment100Counter
	.long	_top_processCpuUsage
	.long	_top_getStatusName
	.long	_top_getRankName
	.long	_top_getPriority
	.long	_top_initialize
	.long	_top_clearTask
	.long	_top_run
	.text
.globl _task_setPriority
	.type	_task_setPriority, @function
_task_setPriority:
	pushl	%ebp
	movl	%esp, %ebp
	cmpl	$0, 8(%ebp)
	je	.L61
	cmpl	$4, 12(%ebp)
	jg	.L61
	cmpl	$0, 12(%ebp)
	js	.L61
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 29(%eax)
.L61:
	leave
	ret
	.size	_task_setPriority, .-_task_setPriority
.globl _task_setRank
	.type	_task_setRank, @function
_task_setRank:
	pushl	%ebp
	movl	%esp, %ebp
	cmpl	$0, 8(%ebp)
	je	.L64
	cmpl	$0, 12(%ebp)
	js	.L64
	cmpl	$1, 12(%ebp)
	jg	.L64
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 25(%eax)
.L64:
	leave
	ret
	.size	_task_setRank, .-_task_setRank
.globl _task_setStatus
	.type	_task_setStatus, @function
_task_setStatus:
	pushl	%ebp
	movl	%esp, %ebp
	cmpl	$0, 8(%ebp)
	je	.L67
	cmpl	$0, 12(%ebp)
	js	.L67
	cmpl	$7, 12(%ebp)
	jg	.L67
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 33(%eax)
.L67:
	leave
	ret
	.size	_task_setStatus, .-_task_setStatus
.globl _task_setESP
	.type	_task_setESP, @function
_task_setESP:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 1085(%eax)
	leave
	ret
	.size	_task_setESP, .-_task_setESP
.globl _task_getPriority
	.type	_task_getPriority, @function
_task_getPriority:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	29(%eax), %eax
	leave
	ret
	.size	_task_getPriority, .-_task_getPriority
.globl _task_getRank
	.type	_task_getRank, @function
_task_getRank:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	25(%eax), %eax
	leave
	ret
	.size	_task_getRank, .-_task_getRank
.globl _task_getStatus
	.type	_task_getStatus, @function
_task_getStatus:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	33(%eax), %eax
	leave
	ret
	.size	_task_getStatus, .-_task_getStatus
.globl _task_getTID
	.type	_task_getTID, @function
_task_getTID:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	leave
	ret
	.size	_task_getTID, .-_task_getTID
.globl _task_getESP
	.type	_task_getESP, @function
_task_getESP:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	1085(%eax), %eax
	leave
	ret
	.size	_task_getESP, .-_task_getESP
.globl _task_setTty
	.type	_task_setTty, @function
_task_setTty:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 1093(%eax)
	leave
	ret
	.size	_task_setTty, .-_task_setTty
.globl _task_getTty
	.type	_task_getTty, @function
_task_getTty:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	1093(%eax), %eax
	leave
	ret
	.size	_task_getTty, .-_task_getTty
.globl _task_runInBackground
	.type	_task_runInBackground, @function
_task_runInBackground:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movb	$1, 1097(%eax)
	leave
	ret
	.size	_task_runInBackground, .-_task_runInBackground
.globl _task_getRunningMode
	.type	_task_getRunningMode, @function
_task_getRunningMode:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movb	1097(%eax), %al
	movsbl	%al,%eax
	leave
	ret
	.size	_task_getRunningMode, .-_task_getRunningMode
.globl _task_setParentTID
	.type	_task_setParentTID, @function
_task_setParentTID:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 1089(%eax)
	leave
	ret
	.size	_task_setParentTID, .-_task_setParentTID
.globl _task_getParentTID
	.type	_task_getParentTID, @function
_task_getParentTID:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	1089(%eax), %eax
	leave
	ret
	.size	_task_getParentTID, .-_task_getParentTID
.globl _task_setSleep
	.type	_task_setSleep, @function
_task_setSleep:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 37(%eax)
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$2
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
	leave
	ret
	.size	_task_setSleep, .-_task_setSleep
.globl _task_decSleep
	.type	_task_decSleep, @function
_task_decSleep:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	37(%eax), %eax
	movl	%eax, %edx
	leal	-1(%eax), %ecx
	movl	8(%ebp), %eax
	movl	%ecx, 37(%eax)
	movl	%edx, %eax
	leave
	ret
	.size	_task_decSleep, .-_task_decSleep
.globl _task_getSleep
	.type	_task_getSleep, @function
_task_getSleep:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	37(%eax), %eax
	leave
	ret
	.size	_task_getSleep, .-_task_getSleep
.globl _task_setSend
	.type	_task_setSend, @function
_task_setSend:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	cmpl	$128, 20(%ebp)
	jle	.L99
	movl	$-1, %eax
	jmp	.L100
.L99:
	movl	8(%ebp), %eax
	leal	45(%eax), %edx
	movl	16(%ebp), %eax
	subl	$4, %esp
	pushl	20(%ebp)
	pushl	%edx
	pushl	%eax
	call	strncpy
	addl	$16, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 37(%eax)
	movl	8(%ebp), %eax
	movl	20(%ebp), %edx
	movl	%edx, 41(%eax)
	movl	8(%ebp), %eax
	movl	$4, 33(%eax)
	movl	$0, %eax
.L100:
	leave
	ret
	.size	_task_setSend, .-_task_setSend
.globl _task_setRecv
	.type	_task_setRecv, @function
_task_setRecv:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	$0, 37(%eax)
	movl	8(%ebp), %eax
	movl	$3, 33(%eax)
	movl	$0, %eax
	leave
	ret
	.size	_task_setRecv, .-_task_setRecv
.globl _task_getByName
	.type	_task_getByName, @function
_task_getByName:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, -16(%ebp)
	jmp	.L105
.L108:
	movl	-16(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L106
	movl	-12(%ebp), %eax
	addl	$4, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	8(%ebp)
	call	streq
	addl	$16, %esp
	testl	%eax, %eax
	je	.L106
	movl	-12(%ebp), %eax
	jmp	.L107
.L106:
	incl	-16(%ebp)
.L105:
	cmpl	$19, -16(%ebp)
	jle	.L108
	movl	$0, %eax
.L107:
	leave
	ret
	.size	_task_getByName, .-_task_getByName
	.section	.rodata
	.align 4
.LC0:
	.string	"Sorry, but there is no room available for more tasks\n"
	.text
.globl _task_findSlot
	.type	_task_findSlot, @function
_task_findSlot:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, -16(%ebp)
	jmp	.L111
.L114:
	movl	-16(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	movl	System+84(%eax), %eax
	testl	%eax, %eax
	sete	%al
	movb	%al, -9(%ebp)
	cmpb	$0, -9(%ebp)
	jne	.L118
.L112:
	incl	-16(%ebp)
.L111:
	cmpl	$19, -16(%ebp)
	jle	.L114
	jmp	.L113
.L118:
	nop
.L113:
	cmpb	$0, -9(%ebp)
	je	.L115
	movl	-16(%ebp), %eax
	jmp	.L116
.L115:
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	movl	$-1, %eax
.L116:
	leave
	ret
	.size	_task_findSlot, .-_task_findSlot
.globl _task_checkTTY
	.type	_task_checkTTY, @function
_task_checkTTY:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	.L120
.L123:
	movl	-4(%ebp), %eax
	addl	$5528, %eax
	movl	System+12(,%eax,4), %eax
	cmpl	8(%ebp), %eax
	jne	.L121
	movl	-4(%ebp), %eax
	jmp	.L122
.L121:
	incl	-4(%ebp)
.L120:
	cmpl	$3, -4(%ebp)
	jle	.L123
	movl	$-1, %eax
.L122:
	leave
	ret
	.size	_task_checkTTY, .-_task_checkTTY
.globl _task_new
	.type	_task_new, @function
_task_new:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, -20(%ebp)
	call	_Cli
	movl	Task+44, %eax
	call	*%eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	addl	$4, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	12(%ebp)
	call	strcpy
	addl	$16, %esp
	movl	Task+52, %eax
	call	*%eax
	movl	8(%ebp), %edx
	movl	%eax, (%edx)
	movl	Task+68, %eax
	subl	$8, %esp
	pushl	32(%ebp)
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	Task, %eax
	subl	$8, %esp
	pushl	24(%ebp)
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	Task+4, %eax
	subl	$8, %esp
	pushl	20(%ebp)
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	8(%ebp), %eax
	movl	$4096, 1081(%eax)
	movl	8(%ebp), %eax
	movl	$0, 1098(%eax)
	movl	Task+100, %eax
	subl	$8, %esp
	pushl	$0
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	System+22576, %eax
	subl	$12, %esp
	pushl	$4096
	call	*%eax
	addl	$16, %esp
	movl	8(%ebp), %edx
	movl	%eax, 1073(%edx)
	movl	8(%ebp), %eax
	movl	1073(%eax), %eax
	leal	4095(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 1077(%eax)
	movl	8(%ebp), %eax
	movl	1073(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	_pageUp
	addl	$16, %esp
	movl	Task+64, %edx
	movl	8(%ebp), %eax
	movl	1077(%eax), %eax
	pushl	%edx
	pushl	36(%ebp)
	pushl	%eax
	pushl	16(%ebp)
	call	_newStack
	addl	$16, %esp
	movl	8(%ebp), %edx
	movl	%eax, 1085(%edx)
	movl	8(%ebp), %eax
	movl	1073(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	_pageDown
	addl	$16, %esp
	cmpl	$0, 28(%ebp)
	jne	.L126
	movl	8(%ebp), %eax
	movb	$0, 1097(%eax)
	movl	Task+68, %eax
	subl	$8, %esp
	pushl	32(%ebp)
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
	jmp	.L127
.L126:
	movl	Task+76, %eax
	subl	$12, %esp
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
.L127:
	cmpl	$0, 28(%ebp)
	jne	.L128
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$1, %eax
	jle	.L128
	movl	Task+84, %edx
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	8(%ebp)
	call	*%edx
	addl	$16, %esp
	movl	$1, -20(%ebp)
	jmp	.L129
.L128:
	movl	Task+84, %edx
	movl	System+76, %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	8(%ebp)
	call	*%edx
	addl	$16, %esp
.L129:
	call	_Sti
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	leave
	ret
	.size	_task_new, .-_task_new
.globl _createTty2
	.type	_createTty2, @function
_createTty2:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	shell
	leave
	ret
	.size	_createTty2, .-_createTty2
.globl _task_kill
	.type	_task_kill, @function
_task_kill:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$52, %esp
	movl	$0, -36(%ebp)
	movl	$4, -12(%ebp)
	call	_Cli
	movl	Task+40, %eax
	subl	$12, %esp
	pushl	8(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jg	.L134
	movl	$0, %eax
	jmp	.L135
.L134:
	movl	Task+96, %edx
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	*%edx
	addl	$16, %esp
	movl	%eax, -40(%ebp)
	movl	System+76, %eax
	cmpl	-16(%ebp), %eax
	je	.L136
	cmpl	$-1, -40(%ebp)
	je	.L137
.L136:
	movl	$1, %eax
	jmp	.L135
.L137:
	movl	$0, -44(%ebp)
	jmp	.L138
.L140:
	movl	Task+88, %ecx
	movl	-44(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	subl	$12, %esp
	pushl	%eax
	call	*%ecx
	addl	$16, %esp
	movl	%eax, -28(%ebp)
	movl	-44(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	movl	System+84(%eax), %eax
	cmpl	$1, %eax
	jle	.L139
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	cmpl	-28(%ebp), %eax
	jne	.L139
	movl	Task+8, %ecx
	movl	-44(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	subl	$8, %esp
	pushl	$6
	pushl	%eax
	call	*%ecx
	addl	$16, %esp
.L139:
	incl	-44(%ebp)
.L138:
	cmpl	$19, -44(%ebp)
	jle	.L140
	movl	Task+88, %eax
	subl	$12, %esp
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	cmpl	$1, %eax
	jle	.L141
	movl	Task+40, %ebx
	movl	Task+88, %eax
	subl	$12, %esp
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	subl	$12, %esp
	pushl	%eax
	call	*%ebx
	addl	$16, %esp
	movl	%eax, -24(%ebp)
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$0
	pushl	-24(%ebp)
	call	*%eax
	addl	$16, %esp
.L141:
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$6
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	-16(%ebp), %eax
	movb	$0, 4(%eax)
	movl	-16(%ebp), %eax
	movl	1073(%eax), %eax
	subl	$8, %esp
	pushl	$1
	pushl	%eax
	call	_sys_free
	addl	$16, %esp
	movl	-16(%ebp), %eax
	movl	$0, (%eax)
	call	_Sti
	call	_scheduler
	movl	$2, %eax
.L135:
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	_task_kill, .-_task_kill
.globl _task_scheduler
	.type	_task_scheduler, @function
_task_scheduler:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	movl	Task+112, %eax
	call	*%eax
	movl	Task+44, %eax
	call	*%eax
	movl	%eax, -16(%ebp)
	call	getNextTask
	movl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	movl	8(%ebp), %edx
	movl	%edx, 1085(%eax)
	movl	Task+24, %eax
	subl	$12, %esp
	pushl	-12(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	%eax, %ebx
	movl	Task+24, %eax
	subl	$12, %esp
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	cmpl	%eax, %ebx
	je	.L144
	movl	-12(%ebp), %eax
	movl	%eax, System+80
	subl	$8, %esp
	pushl	-16(%ebp)
	pushl	-12(%ebp)
	call	switchTTY
	addl	$16, %esp
	movl	-16(%ebp), %eax
	movl	1073(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	_pageDown
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movl	1073(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	_pageUp
	addl	$16, %esp
	movl	-16(%ebp), %eax
	movl	33(%eax), %eax
	cmpl	$7, %eax
	jne	.L145
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$0
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
.L145:
	movl	System+76, %eax
	cmpl	-12(%ebp), %eax
	je	.L144
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$7
	pushl	-12(%ebp)
	call	*%eax
	addl	$16, %esp
.L144:
	movl	System+22540, %edx
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	addl	$5532, %edx
	movl	%eax, System+12(,%edx,4)
	movl	Top, %eax
	call	*%eax
	movl	-12(%ebp), %eax
	movl	1085(%eax), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	_task_scheduler, .-_task_scheduler
.globl _task_maintenance
	.type	_task_maintenance, @function
_task_maintenance:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, -20(%ebp)
	jmp	.L148
.L155:
	movl	-20(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	movl	%eax, -16(%ebp)
	movl	Task+20, %eax
	subl	$12, %esp
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	cmpl	$4, %eax
	je	.L151
	cmpl	$6, %eax
	je	.L152
	cmpl	$2, %eax
	jne	.L149
.L150:
	movl	Task+104, %eax
	subl	$12, %esp
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L157
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$0
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	jmp	.L149
.L151:
	movl	Task+40, %edx
	movl	-16(%ebp), %eax
	movl	37(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	*%edx
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	.L158
	movl	-12(%ebp), %eax
	movl	33(%eax), %eax
	cmpl	$3, %eax
	jne	.L159
	movl	-16(%ebp), %eax
	movl	(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 37(%eax)
	movl	-16(%ebp), %eax
	movl	41(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 41(%eax)
	movl	-16(%ebp), %eax
	movl	41(%eax), %edx
	movl	-12(%ebp), %eax
	addl	$45, %eax
	movl	-16(%ebp), %ecx
	addl	$45, %ecx
	subl	$4, %esp
	pushl	%edx
	pushl	%eax
	pushl	%ecx
	call	strncpy
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movl	$0, 33(%eax)
	movl	-12(%ebp), %eax
	movl	33(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, 33(%eax)
	jmp	.L149
.L152:
	movl	Task+36, %edx
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	*%edx
	addl	$16, %esp
	jmp	.L149
.L157:
	nop
	jmp	.L149
.L158:
	nop
	jmp	.L149
.L159:
	nop
.L149:
	incl	-20(%ebp)
.L148:
	cmpl	$19, -20(%ebp)
	jle	.L155
	leave
	ret
	.size	_task_maintenance, .-_task_maintenance
.globl switchTTY
	.type	switchTTY, @function
switchTTY:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %eax
	movl	1093(%eax), %edx
	movl	System+16, %eax
	movl	27(%eax), %ecx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	leal	0(,%eax,8), %edx
	addl	%edx, %eax
	movl	%eax, %edx
	sall	$5, %edx
	subl	%eax, %edx
	leal	(%edx,%edx), %eax
	movl	%ecx, ttys+5014(%eax)
	movl	12(%ebp), %eax
	movl	1093(%eax), %edx
	movl	System+16, %eax
	movl	31(%eax), %ecx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	leal	0(,%eax,8), %edx
	addl	%edx, %eax
	movl	%eax, %edx
	sall	$5, %edx
	subl	%eax, %edx
	leal	(%edx,%edx), %eax
	movl	%ecx, ttys+5018(%eax)
	movl	System+16, %ecx
	movl	8(%ebp), %eax
	movl	1093(%eax), %edx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	leal	0(,%eax,8), %edx
	addl	%edx, %eax
	movl	%eax, %edx
	sall	$5, %edx
	subl	%eax, %edx
	leal	(%edx,%edx), %eax
	movl	ttys+5014(%eax), %eax
	movl	%eax, 27(%ecx)
	movl	System+16, %ecx
	movl	8(%ebp), %eax
	movl	1093(%eax), %edx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	leal	0(,%eax,8), %edx
	addl	%edx, %eax
	movl	%eax, %edx
	sall	$5, %edx
	subl	%eax, %edx
	leal	(%edx,%edx), %eax
	movl	ttys+5018(%eax), %eax
	movl	%eax, 31(%ecx)
	movl	System+16, %ecx
	movl	8(%ebp), %eax
	movl	1093(%eax), %edx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	leal	0(,%eax,8), %edx
	addl	%edx, %eax
	movl	%eax, %edx
	sall	$5, %edx
	subl	%eax, %edx
	leal	(%edx,%edx), %eax
	addl	$1008, %eax
	addl	$ttys, %eax
	addl	$6, %eax
	movl	%eax, 19(%ecx)
	leave
	ret
	.size	switchTTY, .-switchTTY
.globl _task_getByTID
	.type	_task_getByTID, @function
_task_getByTID:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -8(%ebp)
	jmp	.L163
.L166:
	movl	-8(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	movl	System+84(%eax), %eax
	cmpl	8(%ebp), %eax
	sete	%al
	movb	%al, -1(%ebp)
	cmpb	$0, -1(%ebp)
	jne	.L170
.L164:
	incl	-8(%ebp)
.L163:
	cmpl	$19, -8(%ebp)
	jle	.L166
	jmp	.L165
.L170:
	nop
.L165:
	cmpb	$0, -1(%ebp)
	je	.L167
	movl	-8(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	jmp	.L168
.L167:
	movl	$0, %eax
.L168:
	leave
	ret
	.size	_task_getByTID, .-_task_getByTID
.globl _task_getCurrent
	.type	_task_getCurrent, @function
_task_getCurrent:
	pushl	%ebp
	movl	%esp, %ebp
	movl	System+80, %eax
	leave
	ret
	.size	_task_getCurrent, .-_task_getCurrent
.globl idle
	.type	idle, @function
idle:
	pushl	%ebp
	movl	%esp, %ebp
.L174:
	jmp	.L174
	.size	idle, .-idle
.globl _task_getNewTID
	.type	_task_getNewTID, @function
_task_getNewTID:
	pushl	%ebp
	movl	%esp, %ebp
	movl	tid.1965, %eax
	incl	%eax
	movl	%eax, tid.1965
	movl	tid.1965, %eax
	leave
	ret
	.size	_task_getNewTID, .-_task_getNewTID
	.type	_task_cleaner, @function
_task_cleaner:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	_Cli
	movl	Task+44, %eax
	call	*%eax
	movl	%eax, -16(%ebp)
	movl	Task+88, %eax
	subl	$12, %esp
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	cmpl	$1, %eax
	jle	.L179
	movl	Task+40, %ebx
	movl	Task+88, %eax
	subl	$12, %esp
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	subl	$12, %esp
	pushl	%eax
	call	*%ebx
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$1, %eax
	jle	.L179
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$0
	pushl	-12(%ebp)
	call	*%eax
	addl	$16, %esp
.L179:
	movl	-16(%ebp), %eax
	movb	$0, 4(%eax)
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$6
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	-16(%ebp), %eax
	movl	$0, (%eax)
	movl	-16(%ebp), %eax
	movl	1073(%eax), %eax
	subl	$8, %esp
	pushl	$1
	pushl	%eax
	call	_sys_free
	addl	$16, %esp
	movl	-16(%ebp), %eax
	movl	1098(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	_sys_free_mem
	addl	$16, %esp
	movl	Top+24, %edx
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	*%edx
	addl	$16, %esp
	call	_Sti
	call	_scheduler
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	_task_cleaner, .-_task_cleaner
	.section	.rodata
.LC1:
	.string	""
.LC2:
	.string	"Idle"
	.text
.globl _task_setupScheduler
	.type	_task_setupScheduler, @function
_task_setupScheduler:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	Task+28, %eax
	call	*%eax
	movl	%eax, -12(%ebp)
	cmpl	$-1, -12(%ebp)
	je	.L185
.L182:
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	movl	%eax, -16(%ebp)
	movl	Task+32, %eax
	pushl	$.LC1
	pushl	$-1
	pushl	$1
	pushl	$4
	pushl	$1
	pushl	$idle
	pushl	$.LC2
	pushl	-16(%ebp)
	call	*%eax
	addl	$32, %esp
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$1
	pushl	-16(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	-16(%ebp), %eax
	movl	%eax, System+76
	movl	System+76, %eax
	movl	%eax, System+80
	movl	Top+20, %edx
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	*%edx
	addl	$16, %esp
	movb	$1, scheduling
	nop
	jmp	.L184
.L185:
	nop
.L184:
	leave
	ret
	.size	_task_setupScheduler, .-_task_setupScheduler
.globl _task_yield
	.type	_task_yield, @function
_task_yield:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	call	_Cli
	movl	Task+44, %eax
	call	*%eax
	movl	%eax, -12(%ebp)
	movl	Task+8, %eax
	subl	$8, %esp
	pushl	$0
	pushl	-12(%ebp)
	call	*%eax
	addl	$16, %esp
	call	_Sti
	call	_scheduler
	leave
	ret
	.size	_task_yield, .-_task_yield
.globl _top_increment100Counter
	.type	_top_increment100Counter, @function
_top_increment100Counter:
	pushl	%ebp
	movl	%esp, %ebp
	movl	System+22540, %eax
	incl	%eax
	movl	%eax, System+22540
	movl	System+22540, %eax
	cmpl	$99, %eax
	jle	.L191
	movl	$0, System+22540
.L191:
	leave
	ret
	.size	_top_increment100Counter, .-_top_increment100Counter
.globl _top_processCpuUsage
	.type	_top_processCpuUsage, @function
_top_processCpuUsage:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
	movl	$0, -8(%ebp)
	jmp	.L193
.L195:
	movl	-8(%ebp), %eax
	addl	$5532, %eax
	movl	System+12(,%eax,4), %eax
	cmpl	8(%ebp), %eax
	jne	.L194
	incl	-4(%ebp)
.L194:
	incl	-8(%ebp)
.L193:
	cmpl	$99, -8(%ebp)
	jle	.L195
	movl	-4(%ebp), %eax
	leave
	ret
	.size	_top_processCpuUsage, .-_top_processCpuUsage
	.section	.rodata
.LC3:
	.string	"RUNNING"
.LC4:
	.string	"READY"
.LC5:
	.string	"WAITING"
.LC6:
	.string	"DEAD"
	.text
.globl _top_getStatusName
	.type	_top_getStatusName, @function
_top_getStatusName:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	Task+20, %eax
	subl	$12, %esp
	pushl	12(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	cmpl	$7, -12(%ebp)
	jne	.L198
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC3
	call	strcpy
	addl	$16, %esp
	jmp	.L202
.L198:
	cmpl	$0, -12(%ebp)
	jne	.L200
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC4
	call	strcpy
	addl	$16, %esp
	jmp	.L202
.L200:
	cmpl	$1, -12(%ebp)
	jne	.L201
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC5
	call	strcpy
	addl	$16, %esp
	jmp	.L202
.L201:
	cmpl	$6, -12(%ebp)
	jne	.L202
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC6
	call	strcpy
	addl	$16, %esp
.L202:
	leave
	ret
	.size	_top_getStatusName, .-_top_getStatusName
	.section	.rodata
.LC7:
	.string	"SERVER"
.LC8:
	.string	"NORMAL"
	.text
.globl _top_getRankName
	.type	_top_getRankName, @function
_top_getRankName:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	Task+16, %eax
	subl	$12, %esp
	pushl	12(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	.L204
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC7
	call	strcpy
	addl	$16, %esp
	jmp	.L206
.L204:
	cmpl	$1, -12(%ebp)
	jne	.L206
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC8
	call	strcpy
	addl	$16, %esp
.L206:
	leave
	ret
	.size	_top_getRankName, .-_top_getRankName
	.section	.rodata
.LC9:
	.string	"LOW"
.LC10:
	.string	"HIGH"
.LC11:
	.string	"MIN"
.LC12:
	.string	"MEDIUM"
.LC13:
	.string	"MAX"
	.text
.globl _top_getPriority
	.type	_top_getPriority, @function
_top_getPriority:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	Task+12, %eax
	subl	$12, %esp
	pushl	12(%ebp)
	call	*%eax
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	cmpl	$3, -12(%ebp)
	jne	.L208
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC9
	call	strcpy
	addl	$16, %esp
	jmp	.L213
.L208:
	cmpl	$1, -12(%ebp)
	jne	.L210
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC10
	call	strcpy
	addl	$16, %esp
	jmp	.L213
.L210:
	cmpl	$4, -12(%ebp)
	jne	.L211
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC11
	call	strcpy
	addl	$16, %esp
	jmp	.L213
.L211:
	cmpl	$2, -12(%ebp)
	jne	.L212
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC12
	call	strcpy
	addl	$16, %esp
	jmp	.L213
.L212:
	cmpl	$0, -12(%ebp)
	jne	.L213
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC13
	call	strcpy
	addl	$16, %esp
.L213:
	leave
	ret
	.size	_top_getPriority, .-_top_getPriority
.globl _top_initialize
	.type	_top_initialize, @function
_top_initialize:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	.L215
.L216:
	movl	-4(%ebp), %eax
	leal	5532(%eax), %edx
	movl	8(%ebp), %eax
	movl	%eax, System+12(,%edx,4)
	incl	-4(%ebp)
.L215:
	cmpl	$99, -4(%ebp)
	jle	.L216
	leave
	ret
	.size	_top_initialize, .-_top_initialize
.globl _top_clearTask
	.type	_top_clearTask, @function
_top_clearTask:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	.L219
.L221:
	movl	-4(%ebp), %eax
	addl	$5532, %eax
	movl	System+12(,%eax,4), %eax
	cmpl	8(%ebp), %eax
	jne	.L220
	movl	-4(%ebp), %edx
	movl	System+76, %eax
	movl	(%eax), %eax
	addl	$5532, %edx
	movl	%eax, System+12(,%edx,4)
.L220:
	incl	-4(%ebp)
.L219:
	cmpl	$99, -4(%ebp)
	jle	.L221
	leave
	ret
	.size	_top_clearTask, .-_top_clearTask
	.section	.rodata
	.align 4
.LC14:
	.string	"Processes and it's CPU percentage of use\n"
	.align 4
.LC15:
	.string	"%s [%s] \t tid: %d\t priority: %s\t rank: %s\t use: %d%%\n"
.LC16:
	.string	"\n"
	.text
.globl _top_run
	.type	_top_run, @function
_top_run:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$48, %esp
	subl	$12, %esp
	pushl	$.LC14
	call	printf
	addl	$16, %esp
	call	_Cli
	movl	$0, -12(%ebp)
	jmp	.L224
.L226:
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	leal	0(,%eax,4), %ecx
	addl	%ecx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	movl	System+84(%eax), %eax
	testl	%eax, %eax
	je	.L225
	movl	Top+8, %ecx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	subl	$8, %esp
	pushl	%eax
	leal	-22(%ebp), %eax
	pushl	%eax
	call	*%ecx
	addl	$16, %esp
	movl	Top+12, %ecx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	subl	$8, %esp
	pushl	%eax
	leal	-32(%ebp), %eax
	pushl	%eax
	call	*%ecx
	addl	$16, %esp
	movl	Top+16, %ecx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$System+84, %eax
	subl	$8, %esp
	pushl	%eax
	leal	-42(%ebp), %eax
	pushl	%eax
	call	*%ecx
	addl	$16, %esp
	movl	Top+4, %ecx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	movl	System+84(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	*%ecx
	addl	$16, %esp
	movl	%eax, %ecx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	leal	0(,%eax,4), %ebx
	addl	%ebx, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	movl	System+84(%eax), %ebx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	addl	%edx, %eax
	leal	0(,%eax,4), %esi
	addl	%esi, %eax
	leal	0(,%eax,4), %esi
	addl	%esi, %eax
	sall	%eax
	addl	%edx, %eax
	sall	%eax
	addl	$80, %eax
	addl	$System, %eax
	leal	8(%eax), %edx
	subl	$4, %esp
	pushl	%ecx
	leal	-32(%ebp), %eax
	pushl	%eax
	leal	-42(%ebp), %eax
	pushl	%eax
	pushl	%ebx
	leal	-22(%ebp), %eax
	pushl	%eax
	pushl	%edx
	pushl	$.LC15
	call	printf
	addl	$32, %esp
.L225:
	incl	-12(%ebp)
.L224:
	cmpl	$19, -12(%ebp)
	jle	.L226
	subl	$12, %esp
	pushl	$.LC16
	call	printf
	addl	$16, %esp
	call	_Sti
	movl	$0, %eax
	leal	-8(%ebp), %esp
	addl	$0, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
	.size	_top_run, .-_top_run
	.local	tid.1965
	.comm	tid.1965,4,4
	.ident	"GCC: (Ubuntu 4.4.3-4ubuntu5) 4.4.3"
	.section	.note.GNU-stack,"",@progbits
