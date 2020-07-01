---
title: "PAF Day 9: The Clock Is Ticking"
author: ChatPion
category: devlog
tags: [mapgen, AI, assembly]
layout: post
date: 2020-06-29 23:00:00 +0200
---

Only three days left, it's now _game jam time_. It _is_ possible to make a
decent game in less than three days after all!

## Floor generation

I worked on floor generation. First, I quickly wrote my algorithm in python,
then I spent the rest of the day translating it into C. The error
messages from SDCC were sometimes less than useful (I'm looking at you, "error
line 221" when my file is 220 lines long...). Anyway, here are some generated
floors:

<img src="/gboi-kirby/assets/img/floor/floor1.png" width="200"  /> 
<img src="/gboi-kirby/assets/img/floor/floor2.png" width="200"  /> 
<img src="/gboi-kirby/assets/img/floor/floor3.png" width="200"  /> 

(How to read: `B` = boss room, and numbers indicate the number of existing
neighbours).

I intend on making a separate post about it when I have enough time.

## AI

[AdameBF](https://github.com/AdameBF) finally got his simple AI working. How
intelligent is it? Well it can... die?

<video autoplay height="300" controls>
  <source src="/gboi-kirby/assets/babysteps2.webm" type="video/webm">
</video>



## Mixing C and assembly

After randomly poking around for a while, I stumbled upon how to modify global
variables in inline assembly. Here's some minimal code:

```c
UINT8 var = 42;
static UINT8 static_var = 0;  // it works with static variables too

UINT8 main(void) {
    __asm__("ld hl,#_var");  // met la valeur de var dans static_var
    __asm__("ld a,(hl)");
    __asm__("ld hl,#_static_var");
    __asm__("ld (hl),a");
    printf("%d %d\n", var, static_var);  // prints 42 42
    return 0;
}
```

Note: `static` variables aren't directly callable in the debugger (in Sameboy,
`print #_var` would work, but `print #_static_var`) wouldn't.
