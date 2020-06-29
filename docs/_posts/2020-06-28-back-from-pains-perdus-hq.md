---
title: "PAF Days 6, 7, 8: Back from Pains Perdus HQ"
author: yberreby
category: devlog
tags: []
layout: post
date: 2020-06-28 20:00 +0200
---


Before we dive into the technical side of things, and because what makes a
project is the team behind it, it is worth mentioning that the last few days
were a bit special. Until then, two-thirds of our six-person team had been
collaborating IRL in an Airbnb we rented specifically to work on this
project. Our goal of smoothening out the development process by providing an
enjoyable collaborative environment shared with some of our dear
competitors/friends from Team Rocket was achieved, but all good things must come
to an end, and on Friday, we said goodbye to what would henceforth be known as
Piney's birthplace, and crawled back into the familiarity of our student
apartments as we mentally prepared for the last sprint to the Thursday
deadline...

Here's a summary of what was achieved since then:

* We switched our main ROM from `gbdk-n` to `gbdk-2020` as planned. It works,
  inline assembly can be used (like so: `__asm__("ld d, d")`), but some oddities
  were discovered: for example, using `inline` on functions causes our
  compilation times to explode. We're talking 15 seconds for a few hundreds of
  lines! The cause is unknown as of yet; uncovering it might require a deep dive
  into SDCC, which would take more time than we can spare, so we might just have
  to bite the bullet and pay the price of function calls or use macros.

* [@ChatPion][1] and  [@PaulVez][3] respectively got tile-based collision
  handling and the dashing feature up and running. They were working on their
  own separate branches, and I ([@yberreby][4]) worked on integrating their
  code, which proved excruciating Game Boy programming comes with a lot of
  limitations that make writing clean, modular and _efficient_ code extremely
  hard. A significant number of hours were spent trying to get bugs out, and the
  movement was so glitchy that things were downright comical at some points, but
  we eventually got everything working, and [published our very first
  release][7]! The provided `pod.gb` was tested under BGB:

  <video autoplay controls src="/gboi-kirby/assets/0.1-release-demo.webm" height="400" />

* [@RoussyE][6] has been making solid progress on wrapping his head around the
  Game Boy sound system, [@Ametheek][2] on enemy design, and [@AdameBf][5] on
  enemy handling code. Their work should be detailed in upcoming blog posts!


[1]: https://github.com/ChatPion
[2]: https://github.com/Ametheek
[3]: https://github.com/PaulVez
[4]: https://github.com/yberreby
[5]: https://github.com/AdameBf
[6]: https://github.com/RoussyE
[7]: https://github.com/PainsPerdus/gboi-kirby/releases/tag/0.1
