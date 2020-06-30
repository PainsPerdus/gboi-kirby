---
title: "On Debugging"
author: ChatPion
category: debug
tags: [C, gbdk-2020, assembly, debug]
layout: post
date: 2020-06-28 02:19:42 +0200
---

After fighting with my code for over _four hours_, lo and behold, I finally
managed to have functional collisions! 

<video autoplay height="300" controls>
  <source src="/gboi-kirby/assets/2020-06-28-collisions.webm" type="video/webm">
</video>

EDIT: actually it wasn't working at all, but now, by pouring even more blood,
sweat and tears it does.

## Debugging a gameboy game and C programs

Usually debugging is pretty straightforward: abuse `printf` or its equivalent,
set breakpoints and watch variables. However, as far as I know, there is no
out-of-the-box C debugger for gbdk-2020. I tried using `printf` and at first, it
was quite handy, until I met a strange bug: adding a `printf` did temporarily
fix a bug, and I had absolutely no clues why!

Thankfully, [@JeanMarieMineau](https://github.com/JeanMarieMineau) and
[@g33kex](https://github.com/g33kex) suggested to use
[Sameboy](https://sameboy.github.io) and its debugger by setting breakpoints
using `ld b,b`. After some trial and error, I found out that to insert inline
assembly code I just have to use `__asm__("some assembly code")`
([@yberreby](https://github.com/yberreby) found the syntax `_asm` and `_endasm`
while digging gbdk-2020's source code, but it did not work in my tests). After
doing so, I had to enable software breakpoints by typing `softbreak` in
Sameboy's debugger. Then, it's just a game of figuring out which register
contains which data, taking out the good ol' pen & paper, replicating both the C
code _and_ the assembly code, inevitably miscalculate (yep, `0x20`is _not_ right
after `0x19`!) and slapping my forehead while loudly claiming "oh, that's _why_
it did not work!" as I'm looking at a typo that could have been caught earlier
had I bothered to write some tests...

Writing the game in C instead of assembly surely is handy when it comes to
speed, however it may render debugging more painful as we have less control over
the output assembly code. What's more, that way we entirely depend on gbdk-2020
_not_ having any bugs and I'm afraid to say I did encounter strange behaviours
like the one with `printf`. Hopefully, that choice − switching from assembly to
C − will pay off and not horribly backfire...

## Random things I encountered

- it turns out 324 (18×18) _is_ bigger than 256 ; that made me lose way too much
  time
- additions take precedence over bitwise shifts and I learned that the hard way

It's crazy how small errors made me lose that much time.
