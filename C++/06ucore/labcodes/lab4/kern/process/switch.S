.text
.globl switch_to
switch_to:                      # switch_to(from, to)

    # save from's registers
    movl 4(%esp), %eax          # eax points to from, Since entering the switch_to function will push the EIP on the stack first, 
                                # esp + 4 is required to obtain the 1st parameters: (%eax) = &idleproc.context = 0xc030 a024
                                # eax now points to from
    popl 0(%eax)                # save eip !popl, EIP pushed in when entering switch function
    movl %esp, 4(%eax)          # save esp::context of from
    movl %ebx, 8(%eax)          # save ebx::context of from
    movl %ecx, 12(%eax)         # save ecx::context of from
    movl %edx, 16(%eax)         # save edx::context of from
    movl %esi, 20(%eax)         # save esi::context of from
    movl %edi, 24(%eax)         # save edi::context of from
    movl %ebp, 28(%eax)         # save ebp::context of from

    # restore to's registers
    movl 4(%esp), %eax          # not 8(%esp): popped return address already, 
                                # esp + 4 is required to obtain the 2nd parameters:(%eax) = &initproc.context = 0xc030 a094
                                # eax now points to to
    movl 28(%eax), %ebp         # restore ebp::context of to  0x0
    movl 24(%eax), %edi         # restore edi::context of to  0x0
    movl 20(%eax), %esi         # restore esi::context of to  0x0
    movl 16(%eax), %edx         # restore edx::context of to  0x0
    movl 12(%eax), %ecx         # restore ecx::context of to  0x0
    movl 8(%eax), %ebx          # restore ebx::context of to  0x0
    movl 4(%eax), %esp          # restore esp::context of to  0xc030 cfb4, now esp points to 0xc030 cfb4(*tf)

    pushl 0(%eax)               # push eip = 0xc010 8a95 = forkret

    ret                         # Pop up the stack top element pointed by ESP as EIP, that is 0xc010 8a95 as EIP

