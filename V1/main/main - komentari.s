	.cpu cortex-m3 // .cpu definiše o kom se jezgru radi, cortex-m3 je naveden u komandi za prevodjenje
	.eabi_attribute 20, 1
	// .eabi_attibute - embeded abi attribute ima strukturu ATRIBUT, VREDNOST
	// ATRIBUT je ili integer koji referiše taj atribut ili simboličko ime koje ga referiše
	
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"main.c" // .file je direktiva kojom se definiše naziv fajla -o komanda
	

	.text // .text je SEKCIJA imena .text - tu se nalazi programski mašinski kod sa instrukcijama
	//-begin- .text -----	
	//-end--- .text -----	

	.global	nepromenjiva // .global SIMBOL - prograšavanje SIMBOL-a za globalni

	.section	.rodata // .section IME_SEKCIJE je direktiva kojom se započinje nova sekcija (samim tim se prethodna sekcija završava) 
	//-begin- .rodata -----	
	.align	2 	// poravnanje na adresu deljivu sa 2 ^ VREDNOST; 
	// To nam odgovara za "nepromenjiva" koja je tipa .word 6, znači treba da bude deljiva sa 4	
	.type	nepromenjiva, %object // .type SIMBOL, TIP_SIMBOLA
	// TIP_SIMBOLA %object označava da se radi o OBJEKTU, a ne o FUNKCIJI
	.size	nepromenjiva, 4 // .size SIMBOL, VELICINA (u B)
nepromenjiva:
	.word	6 // .word VREDNOST - alocira jednu REČ koja je na armu 4B
	//-end--- .rodata -----	

	.global	promenjiva
	
	.data // .data je SEKCIJA - podaci	
	//-begin- .data -----	
	.align	2
	.type	promenjiva, %object
	.size	promenjiva, 4
promenjiva:
	.word	7
	//-end--- .data -----	

	.global	neinicijalizovana

	.bss // .bss je SEKCIJA - gde se nalaze neinicijalizovani podaci
	//-begin- .bss -----	
	.align	2
	.type	neinicijalizovana, %object
	.size	neinicijalizovana, 4
neinicijalizovana:
	.space	4
	//-end--- .bss -----	

/*****************************************************************
///!!! - novija verzija alata je napravila sekciju .bss,
// u staroj verziji se nalaze sledeća linija, koja menja celu .bss sekciju

	.comm neinicijalizovana 4,4
	// .comm PROMENJIVA, VELICINA, PORAVNANJE
	// .comm je zajednička promenjiva ako u još nekom fajlu postoji definican simbol PROMENJIVA biće alociran samo jedan zajednički prostor, ne za svaki pojedinačno
	// ovo je definitivno slaba definicija


// to se prevazilazi tako što je umesto 

int neinicijalizovana;

//dodata specifikacija __attribute__ koja definiše kakav je ovaj simbol, pošto je common onda će ga smestiti u common sekciju

int __attribute__((common)) neinicijalizovana;

- iskorišćena je dirigovana dodela svojstva common atributu, da bi on bio u željenoj common sekciji

******************************************************************/	

	.text	
	//-begin- .text -----		
	.align	1 // deljivost sa 2^1 je potrebno jer su sve instrukcije iz thumb veličine 2B
	.global	main 
	.arch armv7-m // .arch ARHITEKTURA - definiše arhitekturu; arhmv7-m je armova arhitektura za mikrokontrolere
	.syntax unified // .syntax IME_SINTAKSE - definiše ime sintaks; unified arm i thumb pišu u ovoj sintaksi
	.thumb // .thumb označava da je odavde pa na dole sve THUMB kod
	.thumb_func // .thumb_func označava tip prvog narednog simbola - thumb funkcija
	.fpu softvfp // .fpu NAČIN - floating-point-unit specificira način obavljanja ovih operacija
	.type	main, %function // tip "main" je finkcija, a ne objekat
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	add	r7, sp, #0
	movs	r3, #6
	adds	r2, r3, #5
	ldr	r3, .L3
	ldr	r3, [r3]
	add	r3, r3, r2
	ldr	r2, .L3+4
	str	r3, [r2]
	movs	r3, #0
	mov	r0, r3
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L4:
	.align	2
.L3:
	.word	promenjiva
	.word	neinicijalizovana
	.size	main, .-main
	.ident	"GCC: (xPack GNU Arm Embedded GCC, 64-bit) 10.2.1 20201103 (release)"
