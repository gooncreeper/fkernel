#ifndef ASM_H
#define ASM_H

#define ASM_GLOBAL_FUNC_DL(label) \
.global label            ;\
.type   label, @function ;\
label:                   ;

#define ASM_GLOBAL_FUNC(label) \
ASM_GLOBAL_FUNC_DL(     label) \
ASM_GLOBAL_FUNC_DL(_ ## label)

#define ASM_GLOBAL_VAR_DL(label) \
.global label ;\
label:        ;

#define ASM_GLOBAL_VAR(label) \
.data                         \
ASM_GLOBAL_VAR_DL(     label) \
ASM_GLOBAL_VAR_DL(_ ## label) \

#endif //ASM_H
