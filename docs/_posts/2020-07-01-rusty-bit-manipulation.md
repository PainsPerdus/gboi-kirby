---
title: "Packing maps with blood, sweat... and Rust :)"
author: yberreby
category: devlog
tags: [mapgen, tooling]
layout: post
date: 2020-07-01 13:04:00 +0200
---

Time marches on relentlessly as the deadline draws closer, but between obscure
compiler errors, linker issues, and hard-to-integrate changes, there is light.
And by *light*, of course, I mean opportunities to write Rust code!
Introducing: `podpacker`, our map compression tool.


## Optimizing map storage

Our plan for map generation was to design chunks by hand, then use them as
building blocks to procedurally generate a map. Last week, we had decided that
we wanted the said chunks to be stored in the ROM in a format that is _as
compact as possible_, alignment be damned. That format would coexist with two
others: an unpacked runtime representation, that takes up more space but is
easier to use from our C code, and a high-level, JSON-based,
Ogmo-Editor-3-compatible representation to let our map designers work
comfortably instead of having to flip bit 7 of the 33rd byte of a chunk by hand
to indicate that there's a top door. *Or was it bit 6?...* You see the issue.

To carry out this plan, we needed a reliable tool to perform the
JSON-to-packed-binary conversion. I volunteered to write it, because since this
was a compile-time utility, I could use whatever language I wanted, and I had
been dying to get some good Rust-writing time in.

On Tuesday, the tireless [@ChatPion][1] produced a [specification][2] of the
two map formats I would be working with, and I was able to begin working on
`podpacker` - the official *Pineapple of Doom Map Packer*.

*NB: It still lives under the name `ogmo-parser` on `master` at the moment, but
that should change once a few more PRs are merged...*


## Working smarter, not harder

Some say that one of the greatest qualities of a programmer is laziness, in
which case I am undoubtedly a 10x programmer. Not wanting to write a lot of
fragile JSON-parsing code, I wanted to use  `serde`, the *de facto* standard
Rust library for **ser**ializationb/**de**serialization, which has great support
for JSON and makes parsing it a breeze. You basically just have to specify the
schema and you're up and running, with clean errors and type safety. It's
amazing.

Wanting to push the laziness even further, I looked for an existing Ogmo parser,
and was lucky enough to find [BrianMWest's ogmo-rs][3], which I used as a base.
I had to adapt it somewhat, because:

1) Ogmo's tile layer format has changed slightly since this library was written,
2) `bool`s are not `String`.

I originally forked it, but integrating it directly into the project and
changing a number of details proved to be a much more time-efficient solution.
I would have liked to contribute back, but at the moment, time is of the
essence ; forgive me, gods of FOSS, for I have sinned.

## From raw binary output to C files

My initial release of `podpacker` performed the packing, then just output the
result as-is to `stdout` without bothering to perform any human-readable
formatting. I left diagnostics messages on `stderr`, which allowed for neat
visualization of the resulting data by piping to `xxd -b`.

This would probably have been enough if we had still been working in assembly,
as we could have included binary blobs in the ROM. However, now that we worked
in C, it was more convenient to format the output in a way that was easy to use
from it, so, taking inspiration from [`img2gb`][4] (another neat open-source tool
we use pervasively through this project), it was decided that `podpacker` would
generate a C source file and correspondign header file, to make integration a
breeze.

[@ChatPion][1] put the final touches, some bugs were fixed, and now we have
working map generation!

[1]: https://github.com/ChatPion 
[2]: https://github.com/PainsPerdus/gboi-kirby/wiki/Map-specifications
[3]: https://github.com/BrianMWest/ogmo-rs
[4]: https://github.com/flozz/img2gb
