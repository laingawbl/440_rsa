
#include "rsa.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

void msgtest(){

    rsa_pub ke; 
    rsa_pri kd;

    int j;

    char *msg = "\
All human beings are born free and equal in dignity and rights. \
They are endowed with reason and conscience and should act towar\
ds one another in a spirit of brotherhood. Everyone is entitled \
to all the rights and freedoms set forth in this Declaration, wi\
thout distinction of any kind, such as race, colour, sex, langua\
ge, religion, political or other opinion, national or social ori\
gin, property, birth or other status. Furthermore, no distinctio\
n shall be made on the basis of the political, jurisdictional or\
 international status of the country or territory to which a per\
son belongs, whether it be independent, trust, non-self-governin\
g or under any other limitation of sovereignty. Everyone has the\
 right to life, liberty and the security of person. No one shall\
 be held in slavery or servitude; slavery and the slave trade sh\
all be prohibited in all their forms. No one shall be subjected \
to torture or to cruel, inhuman or degrading treatment or punish\
ment. Everyone has the right to recognition everywhere as a pers\
on before the law. All are equal before the law and are entitled\
 without any discrimination to equal protection of the law. All \
are entitled to equal protection against any discrimination in v\
iolation of this Declaration and against any incitement to such \
discrimination. Everyone has the right to an effective remedy by\
 the competent national tribunals for acts violating the fundame\
ntal rights granted him by the constitution or by law. No one sh\
all be subjected to arbitrary arrest, detention or exile. Everyo\
ne is entitled in full equality to a fair and public hearing by \
an independent and impartial tribunal, in the determination of h\
is rights and obligations and of any criminal charge against him\
. Everyone charged with a penal offence has the right to be pres\
umed innocent until proved guilty according to law in a public t\
rial at which he has had all the guarantees necessary for his de\
fence. No one shall be held guilty of any penal offence on accou\
nt of any act or omission which did not constitute a penal offen\
ce, under national or international law, at the time when it was\
 committed. Nor shall a heavier penalty be imposed than the one \
that was applicable at the time the penal offence was committed.\
No one shall be subjected to arbitrary interference with his pri\
vacy, family, home or correspondence, nor to attacks upon his ho\
nour and reputation. Everyone has the right to the protection of\
 the law against such interference or attacks. Everyone has the \
right to freedom of movement and residence within the borders of\
 each State. Everyone has the right to leave any country, includ\
ing his own, and to return to his country. Everyone has the righ\
t to seek and to enjoy in other countries asylum from persecutio\
n. This right may not be invoked in the case of prosecutions gen\
uinely arising from non-political crimes or from acts contrary t\
o the purposes and principles of the United Nations. Everyone ha\
s the right to a nationality. No one shall be arbitrarily depriv\
ed of his nationality nor denied the right to change his nationa\
lity. Men and women of full age, without any limitation due to r\
ace, nationality or religion, have the right to marry and to fou\
nd a family. They are entitled to equal rights as to marriage, d\
uring marriage and at its dissolution. Marriage shall be entered\
 into only with the free and full consent of the intending spous\
es. The family is the natural and fundamental group unit of soci\
ety and is entitled to protection by society and the State. Ever\
yone has the right to own property alone as well as in associati\
on with others. No one shall be arbitrarily deprived of his prop\
erty. Everyone has the right to freedom of thought, conscience a\
nd religion; this right includes freedom to change his religion \
or belief, and freedom, either alone or in community with others\
 and in public or private, to manifest his religion or belief in\
 teaching, practice, worship and observance. Everyone has the ri\
ght to freedom of opinion and expression; this right includes fr\
eedom to hold opinions without interference and to seek, receive";

    char ciph[SYM_SZ];
    char in[SYM_SZ];
    char out[SYM_SZ];
    int mlen = strlen(msg);
    char *p = msg;

    int ok = 0;
    int bad = 0;

    genkeys(EXP_DEFAULT, 128, &ke, &kd);

    printf("setup keys with %d-bit modulus:\n", highbit(ke.n));
    print_num(ke.n);
    printf("processing message of %d bytes\n", mlen);

    for(;;){
        memset(ciph, 0, SYM_SZ);
        memset(out, 0, SYM_SZ);
        strncpy(in, p, SYM_SZ-4);

        rsa_enc(&ke, in, SYM_SZ-4, ciph);
       
        rsa_dec(&kd, ciph, out);
        
        int match = 1;
        for(j = 0; j < SYM_SZ-4; j++){
            if(out[j] != in[j])
                match = 0;
        }
        if(match)
            ok++;
        else
            bad++;

        printf("%s\n", out);
        p += SYM_SZ-4;
        if((p-msg) >= mlen)
            break;
    }

    printf("\n%d symbols OK, %d symbols differ\n", ok, bad);
}

void keytest(){
    Word a[N_SZ];

    int i;
    
    for(i=0; i<100; i++){
        load_miller_rabin(64, a);
    }

}

void multest(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    srandom(t.tv_sec ^ t.tv_nsec);
 
    Word a[N_SZ];
    Word b[N_SZ];
    Word hi[N_SZ];
    Word lo[N_SZ];
    Word q[N_SZ];
    Word r[N_SZ];

    zero(a);
    zero(b);
    zero(q);
    zero(r);

    load_random(N_SZ, a);
    load_random(N_SZ, b);

    print_num(a);
    print_num(b);
    mul(a, b, hi, lo);
    printf("mul\n");
    print_num(hi);
    print_num(lo);

    qdiv(hi, b, q, r);
    printf("div hi\n");
    print_num(q);
    print_num(r);

    qdiv(lo, b, q, r);
    printf("div lo\n");
    print_num(q);
    print_num(r);
}

int main(){
    msgtest();
    return 0;
}
