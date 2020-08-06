---
title: "PAF Postmortem: to C or not to C?"
author: ChatPion
category: devlog
tags: [C, assembly]
layout: post
date: 2020-07-02 18:53:02 +0200
---

_Note : this post was originally intended to be written shortly after the PAF ended, but its writing had been postponed several times due to life happening. I left the original date not to overshadow the last team blog post. Anyway, one month later (2020/08/09), here it is!_

_Note : I am **not** speaking − or rather writing − on behalf of the team. The thoughts and opinions expressed here are mine and may not be shared by the whole team._

Finally, the PAF is officially done and with that our official school year too. Before closing this blog, I would like to come back to the biggest choice that we made : switching from gameboy assembly to C.

## Working with C

One or two days after the beginning, [@yberreby](https://github.com/yberreby) found [a tutorial for gbdk-n](https://blog.flozz.fr/2018/10/01/developpement-gameboy-1-hello-world/) that worked pretty well and after consulting wtih the rest of the group, we concluded it wouldn't hurt to try. Our main reason for choosing C was to iterate fast. Some team members were also not confident in writing assembly and welcomed the change with open arms. Personally, I would have prefered to stay with assembly, but prefered not to go against the team.


### Writing clean code

Theoretically, by using C, our codebase should have been easier to maintain and refactor. Except that we were working on a _gameboy_ game, and the processor of the gameboy heavily relies on registries, whereas C is a stack-based language. Therefore, functions and local variables went to the trashbin and global variables were kings.

Since we did not impose many coding guidelines, we ended up with tons of spaghetti-code I'm not proud of. It's ugly. It's poorly organized. It's poorly documented. It looks like it was made for a game jam (which could have been true given the amount of code we wrote during the last three days). And the logic is completely _bound_ to the graphics, it's truly terrible in my opinion. I felt dirty writing it. I _hate_ it. I don't want to touch it again even with a ten-foot pole. Worst part, most of the code I hate is code that _I_ wrote.

Thankfully, some wrote more decent code, like [@AdameBF](https://github.com/AdameBF) who wrote _OOP-ish_ code that was less painful to use. I, on the other hand, am guilty of having written poor-quality undocumented code, but for my defense, it was a matter of either having ugly but functional game or missing tons of core features.


### Makefile issues

Because we did not take enough time to setup a nice and clean Makefile, we ended up with two problems.

Firstly, our Makefile was probably poorly written and broken. At some point, we couldn't rely on a simple `make` to build our project, but had to run `make
clean` before rebuilding all of our assets. This did not significatively slow us down, but was mildly annoying nonetheless.

Secondly, our source code directory was completely _cluttered_ with output files from the compiler. Again, this wasn't a breaking issue, but still a mildly annoying one. Finding one particular file became more and more painful as we split our codebase into multiple files.

<img src="/gboi-kirby/assets/img/cluttered_src.png" width="200" /> 

(only the files in white interested us, the rest are byproducts of compilation)


### My conclusions

Was it an error? I did encounter [some]({% post_url 2020-06-28-on-debugging %}) [problems]({% post_url 2020-07-01-mapgen-part2 %}), but they were avoidable.

In retrospect, it did not work as well as we initially thought. If we take a look at the timeline, because we had to start from scratch and learn how gdbk-2020 works, most features were implemented during the last three days, even _core_ features such as the chainsaw attack were added during the last day! Nonetheless I believe C is not the main culprit here, but rather the huge discrepancies of skill and motivation levels within our team. When we created the two teams, we thought that they were balanced, but looking back, that wasn't really the case. On the technical side perhaps that was true, although I'm not that sure anymore,  however, motivation-wise the other team completely blew us out of the water, there is no denying that. I still remember those crazy nights during which they were still up coding and doing crazy shenanigans to bypass the gameboy's restrictions, like their incredible title screen,  while most of my team was asleep − not that I blame them for having a healthy sleep schedules.

In the end, I do feel like we might have missed the whole challenge of coding a gameboy game by choosing the C and gdbk-2020 combo. gbdk-2020 does such a good job abstracting away the gameboy that coding this project felt very similar to coding another game (if I disregard all the gameboy-related bugs I encountered), albeit I definitely would have chosen a more sensible tech stack, language-wise and engine-wise. The other team was _obsessed_ with optimizations and cycle counts, for example they told us that some parts of their code needed to run under X cycles, otherwise the rest would be skipped, while we just called `wait_for_vblank()` and called it a day. I kind of regret not having this kind of problems because _that_ was the challenge I signed up for, not fighting against a stupid collision algorithm! Yep, one month later I'm still salty about that. I read in the past articles about games which had to fit small memory cartidges and the tricks that the devs had to come up with. I was _ecstatic_ at the idea of doing the same. And I did! I'm pretty happy of my map generation algorithm which only requires chunks of 33 bytes of data. But I looking back, I wish I faced other challenges of the same type. I did learn one or two things about C though, so there's that.

To conclude, if I were to make another gameboy game, I don't think I would pick C again, but rather stay with assembly.

## What about this project now?

Frankly, I'm sick of its messy code and its messy bugs. But, at the very least, I'd like to fix the enemies not being correctly rendered and add inter-floor movement, then call it a day and go on my merry way.
