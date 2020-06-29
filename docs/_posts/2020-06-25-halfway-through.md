--- 
title: "PAF Day 5: Halfway through"
author: ChatPion 
category: devlog 
tags: [tooling, graphics, gameplay, jekyll] 
layout: post
date: 2020-06-25 12:00 +0200
---

Today we are exactly halfway through this project's allocated time − time
really flies when you're having fun! Anyway, here's today's program: struggling
with Jekyll, struggling with gbdk-2020, map generation, more character
movements, and graphics.

## Blogging with Jekyll

Jekyll is really handy for blogging, but for some reason, it fully displayed
[@Ametheek's post]({% post_url 2020-06-25-some-explanation-on-graphics %})
instead of only showing its excerpt. What's more, the contents of her post (in
Markdown) looked exactly like the other posts. After some digging, it turns out
that Jekyll does _not_ like Windows-style line endings (CRLF - `\r\n`).
It may be possible to configure Jekyll to avoid this problem, but frankly, I
don't want to spend more time setting this website up...

## Struggling with gbdk-2020

Switching from assembly to gbdk-n to gbdk-2020 was not an easy feat. While some
of us did not have any problems compiling gbdk-2020 and running the include
examples, [@RoussyE](https://github.com/RoussyE) couldn't even compile it.
Fortunately, [@nazebzurati's
comment](https://github.com/Zal0/gbdk-2020/issues/28#issuecomment-649496832)
fixed his problems. He could finally run gbdk-2020's sound example and listen to
_Twinkle twinkle little star_ on an emulator. Hopefully, he'll be able to
decipher the sample's source code and play music on his own.

## Map generation

I began to implement the room generation algorithm in C. It's funny how I have
to be wary of everything: inlining functions as often as possible (and avoid
stack overflows), using as few bits as possible and in general, being 
extremely aware of the amount of memory I'm using. Worst case, I'll have 
to write some parts in assembly if optimizations are required. If I'm lucky, 
I may be able to generate rooms tomorrow.

## Character movements

By refactoring the original code a bit, we were able to easily add diagonal
movements. For the time being, we will not create animations for this because
the number of required animations would _explode_. Now, next step: dashing!

## More assets

[@Ametheek](https://github.com/Ametheek) began to work on background tiles:

<img src="/gboi-kirby/assets/img/background/ground.png" width="32"  /> 
<img src="/gboi-kirby/assets/img/background/hole.png" width="32"  /> 
<img src="/gboi-kirby/assets/img/background/pikes.png" width="32"  /> 
<img src="/gboi-kirby/assets/img/background/stone.png" width="32"  /> 
<img src="/gboi-kirby/assets/img/background/wall_0.png" width="32"  /> 
<img src="/gboi-kirby/assets/img/background/wall_1.png" width="32"  /> 
<img src="/gboi-kirby/assets/img/background/wall_2.png" width="32"  /> 

There is more from where that came from (like a **glorious** title screen), but
that will be showcased when the time comes ;)
