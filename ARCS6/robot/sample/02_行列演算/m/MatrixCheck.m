% 行列演算チェック用MATLABコード
% Yokokura, Yuki	2020/06/07
clc;
clear;

Aqr = [
	12, -51,   4;
	 6, 167, -68;
	-4,  24, -41;
];
[Qqr, Rqr] = qr(Aqr)

Aqr = [
	12, -51,   4, 39;
	 6, 167, -68, 22;
	-4,  24, -41  11;
];
[Qqr, Rqr] = qr(Aqr)
[Qqr, Rqr] = qr(Aqr.')

Aqr = [
	10, -8,  5;
	-8,  9,  6;
	-1,-10,  7;
];
[Qqr, Rqr] = qr(Aqr)

Acomp3 = [
  4 + 6i,  1 - 3i,  5 + 2*i ;
  8 - 5i, -7 - 6i,  7 - 1*i ;
  9 + 9i, -7 - 5i, -5 - 3*i ;
];
inv(Acomp3)

