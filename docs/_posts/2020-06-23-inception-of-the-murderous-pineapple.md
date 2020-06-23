---
title: "PAF Day 3: The Inception of the Murderous Pineapple"
author: yberreby
category: devlog
tags: [tooling, graphics, gameplay]
layout: post
date: 2020-06-23 19:00 +0200
---

Today, our team decided to ~~go rock climbing with Team Rocket~~ build on
yesterday's creative momentum. Our efforts were focused on **tooling**,
**character / weapon design**, and **maps**. This last point is mostly the
result of [@ChatPion](https://github.com/ChatPion) and
[@RoussyE](https://github.com/RoussyE)'s efforts, and will be covered in an
upcoming blog post.
 

## Tooling

When we decided to embark on this journey, our understanding was that our only
options were either to code directly in GBZ80 ASM, or to use an outdated and most
importantly _buggy_ C compiler: a version of SDCC dating all the way back to
2001, bundled with the honorable but decrepit
[GBDK](http://gbdk.sourceforge.net/).

Under those circumstances, the obvious choice was the former. Debugging is going
to be hard enough as is; we can't afford to use an untrustworthy compiler. But
as it turns out, we were mistaken about the current state of the Game Boy
programming ecosystem.

We ran into a [series of blog
posts](https://blog.flozz.fr/2019/09/18/developpement-gameboy-1bis-re-hello-world/)
written by someone who clearly knew what they were doing, and which showed us
the error in our ways. We discovered that SDCC's outstanding GBZ80 bugs had long
been patched, and that there existed an updated version of GDBK,
[`gdbk-n`](https://github.com/flozz/gbdk-n) (the link is towards the blog posts'
author's fork of this library).
Following the impulse of [@Aunsiels](https://github.com/Aunsiels), we also
turned to the [gbdev Discord server](https://discord.gg/zaDtE6y), discovered
that they had a `#gbdk` channel (which should come in quite handy for seeking
support), and from _there_, discovered an even better refresh of the original
GBDK: [`gbdk-2020`](https://github.com/Zal0/gbdk-2020).

Given that switching to C as our main language would allow us to iterate more
quickly on complex features without preventing us from dropping back to ASM for
hot zones (or for the sake of entertainment :)), we decided to move forward with
that decision; the aforementioned tutorial will serve as a starting point. We'll
adapt it to use `gbdk-2020`, and build upon it.


## Character design

[@Ametheek](https://github.com/Ametheek) started working on a very, very
important part of our game: the sprites, and more specifically the design of
the main character.

Here at Team Kirby, we believe in a "Kill Them with Kindness"
approach to life, so we wanted an endearing main character.


For its design, we eventually landed on incredibly cute pineapple. I mean, look
at it walk! Isn't it adorable?

<img src="/gboi-kirby/assets/img/latwalk1.gif" width="150"  />
<img src="/gboi-kirby/assets/img/downwalk1.gif" width="150"  />


As everyone knows, there is no such thing as a cute pineapple without psychopathic
tendencies and an unquenchable thirst for blood, so we decided to equip the MC with
what is to become its most iconic weapon, a chainsaw. We call the chainsaw _Kindness_.

Here is a very preliminary version of it in action:

<img src="/gboi-kirby/assets/img/chainsaw1.gif" width="150" />

This chainsaw will be a major gameplay component, so we're going to spend time
fine-tuning it. We'll elaborate on how it works and why it's going to be awesome
in an upcoming post.

