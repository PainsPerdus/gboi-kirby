---
title: "On Map Generation (part 2): integrating everything"
author: ChatPion
category: devlog
tags: [mapgen, tooling]
layout: post
date: 2020-07-01 17:47:00 +0200
---

The end is nigh and I did not sleep much last night, because I was too busy
integrating the entire map generation code to the main project. Fortunately, I
managed to do it, here it is in action:

<video autoplay height="300" controls>
  <source src="/gboi-kirby/assets/2020-07-01-mapgen-integration.webm" type="video/webm">
</video>

## The linker did not like me

I was stuck for three long hours because of a tiny error that
[@yberreby](https://github.com/yberreby) fixed in five minutes. 

Basically, I declared a global variable in `global.h` with `FLOOR base_floor;`,
and called it from two separate .c files. However, the linker was not
cooperative and assigned them to two different memory locations, one being
right in the middle of another variable! How do you solve this problem? By
adding the keywork `extern` in the declaration in `global.h`: `extern FLOOR
base_floor`. That's it!

## Writing C code that poorly translates to assembly

Refactoring the room generation code was tedious because compiling took
_forever_ (15 to 20 seconds) compared to compiling the rest. Since I knew I was
going to run it _a lot_, I investigated the resulting assembly code, and oh boy
I wasn't disappointed: 18k lines of code (LOC) for just one file, with one
_line_ of C requiring 10k LOC!! Alarm bells rang in my head: I needed to address
this issue ASAP. Thankfully, by using one simple trick, I managed to reduce the
number of LOC by a crazy amount: from 18k LOC to a mere 1.5k LOC! 

For example, by changing this: 

```c
int main(void) {
    ROOM room = {
        .doors = {
            {0, {0, 0, 0}},
            {0, {0, 0, 0}},
            {0, {0, 0, 0}},
            {0, {0, 0, 0}},
        },
        .is_small = TRUE,
        .small_tiles = {EMPTY}
    };

    ...
};
```

to this:

```c
ROOM room = {
    .doors = {
        {0, {0, 0, 0}},
        {0, {0, 0, 0}},
        {0, {0, 0, 0}},
        {0, {0, 0, 0}},
    },
    .is_small = TRUE,
    .small_tiles = {EMPTY}
};

int main(void) {
    ...
}
```

I managed to save 10k LOC. Yep. Ten thousand lines of code.

### Moral of the story? 

Just like written
[here](https://gbdev.io/to_c_or_not_to_c.html#optimizing-for-gbdk), global
variables should be used preferentially before local variables.
