;on veut faire déplacer une bille sauf que cette fois elle a une taille de 16*16

.ROMDMG                         ;Pas de features CGB
.NAME "PONGDEMO"                ;Nom du ROM inscrit dans le header
.CARTRIDGETYPE 0                ;ROM only
.RAMSIZE 0
.COMPUTEGBCHECKSUM              ;WLA-DX écrira le checksum lui-même (nécessaire sur une vraie GB)
.COMPUTEGBCOMPLEMENTCHECK       ;WLA-DX écrira le code de verif du header (nécessaire sur une vraie GB)
.LICENSEECODENEW "00"           ;Code de license Nintendo, j'en ai pas donc...
.EMPTYFILL $00                  ;Padding avec des 0

.MEMORYMAP
SLOTSIZE $4000
DEFAULTSLOT 0
SLOT 0 $0000
SLOT 1 $4000
.ENDME

.ROMBANKSIZE $4000              ;Deux banks de 16Ko
.ROMBANKS 2

.BANK 0 SLOT 0

.ENUM $C000
BalleX     DB
BalleY     DB
.ENDE

.ORG $0040
call      VBlank                ;L'interruption VBlank tombe ici
reti

.ORG $0100
nop
jp    start                     ;Entry point

.ORG $0104
;Logo Nintendo, obligatoire
.db $CE,$ED,$66,$66,$CC,$0D,$00,$0B,$03,$73,$00,$83,$00,$0C
.db $00,$0D,$00,$08,$11,$1F,$88,$89,$00,$0E,$DC,$CC,$6E,$E6
.db $DD,$DD,$D9,$99,$BB,$BB,$67,$63,$6E,$0E,$EC,$CC,$DD,$DC
.db $99,$9F,$BB,$B9,$33,$3E

.org $0150
start:
  di                            ;Interruption désactivées
  ld     sp,$FFF4               ;Début du stack à $FFF4 parce que Nintendo le veut

  xor a
  ldh    ($26),a                ;Coupe le circuit son

waitvbl:
  ldh    a,($44)                ;Attend le début d'un VBL (première ligne hors de l'écran, Y>144)
  cp     144
  jr     c, waitvbl

  ld     a,%00010001            ;Éteins l'écran
  ldh    ($40),a

  ;Charge 3 tiles
  ld     b,8*3*2                ;3*8 lignes 2BPP
  ld     de,tiles
  ld     hl,$8000
ldt:
  ld     a,(de)
  ldi    (hl),a
  inc    de
  dec    b
  jr     nz,ldt

  ld     de,32*32               ;Vide la map BKG
  ld     hl,$9800
clmap:
  xor    a
  ldi    (hl),a
  dec    de
  ld     a,e
  or     d
  jr     nz,clmap

  ld     hl,$FE00               ;Vide l'OAM
  ld     b,40*4
clspr:
  ld     (hl),$00
  inc    l                      ;Contournement bug hardware
  dec    b
  jr     nz,clspr

  xor    a
  ldh    ($42),a                ;Scroll Y
  ldh    ($43),a                ;Scroll X
  

  ;tile haut gauche
  ld     hl,$FE00
  ld     (hl),$80               ;OAM  Y
  inc    l
  ld     (hl),$80               ;OAM  X
  inc    l
  ld     (hl),$02               ;OAM balle tile
  inc    l
  ld     (hl),0                 ;OAM balle attribut
  
  ;tile haut droite
  ld     hl,$FE04
  ld     (hl),$88               ;OAM  Y
  inc    l
  ld     (hl),$80               ;OAM  X
  inc    l
  ld     (hl),$02               ;OAM balle tile
  inc    l
  ld     (hl),0                 ;OAM balle attribut
  
  ;tile bas gauche
  ld     hl,$FE08
  ld     (hl),$80               ;OAM  Y
  inc    l
  ld     (hl),$88               ;OAM  X
  inc    l
  ld     (hl),$02               ;OAM balle tile
  inc    l
  ld     (hl),0                 ;OAM balle attribut
  
  ; tile bas droite
  ld     hl,$FE0C
  ld     (hl),$88               ;OAM  Y
  inc    l
  ld     (hl),$88               ;OAM  X
  inc    l
  ld     (hl),$02               ;OAM balle tile
  inc    l
  ld     (hl),0                 ;OAM balle attribut

            ;Inits variables
  ld     a,$80
  ld     (BalleX),a
  ld     (BalleY),a


  ld     a,%11100100            ;Palette BG
  ldh    ($47),a
  ldh    ($48),a                ;Palette sprite 0
  ldh    ($49),a                ;Palette sprite 1 (sert pas)
  ld     a,%10010011            ;Allume l'écran, BG on, tiles à $8000
  ldh    ($40),a

  ld     a,%00010000            ;Interruptions VBlank activées
  ldh    ($41),a
  ld     a,%00000001            ;Interruptions VBlank activée (double activation à la con)
  ldh    ($FF),a

  ei

loop:
  jr     loop



VBlank:
  push   af
  push   hl

  ld     a,%00100000            ;Selection touches de direction
  ldh    ($00),a
  
  ldh    a,($00)
  ld     b,a

  bit    $3,b                   ;bas
  jr     nz,nod
  ld     a,(BalleY)
  inc    a
  ld     (BalleY),a
 nod:

  bit    $2,b                   ;Haut
  jr     nz,nou
  ld     a,(BalleY)
  dec    a
  ld     (BalleY),a
 nou:

  bit    $1,b                   ;Gauche
  jr     nz,nod2
  ld     a,(BalleX)
  dec    a
  ld     (BalleX),a
 nod2:

  bit    $0,b                   ;Droite
  jr     nz,nou2
  ld     a,(BalleX)
  inc    a
  ld     (BalleX),a
 nou2:


  ld     hl,BalleX
  ld     a,(hl)


  cp     155     ;160+1-8           ;BalleX < 160: pas de collision mur droit     ; le +1 c'est pour ne pas avoir le bruit en permanence quand la balle est contre le mur
  jr     c,nocxr
  call   lowbeep
  ld     a,154                  ;Limite à 160 -8
nocxr:

  cp     8
  jr     nc,nocxl               ;BalleX > 2: pas de collision mur gauche
  call   lowbeep
  ld     a,8
nocxl:
  ld     (hl),a
  


  ld     hl,BalleY
  ld     a,(hl)

  cp     144+1                ; le +1 c'est pour ne pas avoir en permanence le bruit quand la balle est en bas, on veut le bruit que quand la balle est en bas et qu'on appuie sur la fleche du bas
  jr     c,nocyr                ;Collision bas
  call   lowbeep
  ld     a,144                ;Limite
nocyr:
  cp     8+8
  jr     nc,nocyl               ;Collision haut
  call   lowbeep
  ld     a,8+8                  ;Limite
nocyl:
  ld     (hl),a

  ;haut gauche
  ld     hl,$FE00
  ld     a,(BalleY)
  ld     (hl),a                 ;OAM balle Y
  inc    l
  ld     a,(BalleX)
  ld     (hl),a                 ;OAM balle X
  
  ;bas droit
  ld     hl,$FE04
  ld     a,(BalleY)
  add    8
  ld     (hl),a                 ;OAM balle Y
  inc    l
  ld     a,(BalleX)
  add    8
  ld     (hl),a                 ;OAM balle X
  
  ;bas gauche
  ld     hl,$FE08
  ld     a,(BalleY)
  add    0
  ld     (hl),a                 ;OAM balle Y
  inc    l
  ld     a,(BalleX)
  add    8
  ld     (hl),a                 ;OAM balle X
  
  ;bas droit
  ld     hl,$FE0C
  ld     a,(BalleY)
  add    8
  ld     (hl),a                 ;OAM balle Y
  inc    l
  ld     a,(BalleX)
  add    0
  ld     (hl),a                 ;OAM balle X

  pop    hl
  pop    af
  ret


lowbeep:
  call   setsnd
  ld     a,%00000000
  ldh    ($13),a
  ld     a,%11000111
  ldh    ($14),a
  ret

  
hibeep:
  call   setsnd
  ld     a,%11000000
  ldh    ($13),a
  ld     a,%11000111
  ldh    ($14),a
  ret


setsnd:
  ld     a,%10000000
  ldh    ($26),a

  ld     a,%01110111
  ldh    ($24),a
  ld     a,%00010001
  ldh    ($25),a

  ld     a,%10111000
  ldh    ($11),a
  ld     a,%11110000
  ldh    ($12),a
  ret

  .ORG   $0800
tiles:
  .INCBIN "tiles.bin"
