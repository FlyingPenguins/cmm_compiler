000 0 	;constant 0 at static address 0
2 "\n"
8 "Enter two integers: "
29 "The GCD is "
$ 13 40 ;Start code section, X & Y are in 4 and 6
# 4 		;enter gcd(), a=@/6, b=@/8, tmp0=/-2, tmp2=/-4
e @/8 0 4 	;if b == 0, goto 4
j 6 		;if <>0, goto 6
i @/6 @/4  	;gcd = a
j 12 		;goto return
r @/6 @/8 /-2 ;form tmp0 = a mod b
p #/-2 	;set second parameter = tmp0
p /8 		;set first parameter = b
c #/-4 1 	;call gcd recursively; result to tmp2
^ 4			;pop parameters
i /-4 @/4 	;store return value
/ 		;return from subroutine
# 2 		;enter main; 2 bytes of local storage for tmp0=-2
p #8 	;set up string parameter
c 0 -11 	;output string
^ 2
p #4 	;parameter for Read
c 0 -1 	;read integer to g4 (x)
^ 2
p #6 	;parameter for Read
c 0 -1 	;read integer to g6 (y)
^ 2
p #29 	;message as parameter
c 0 -11 	;write string
^ 2
p #6 	;set second parameter = y
p #4 	;set first parameter = x
c #/-2 1 	;call gcd result to tmp0
^ 4
p #/-2 	;set parameter for write = tmp0
c 0 -9 	;write integer
^ 2
p #2 	;string address
c 0 -11 	;write string
^ 2
h

