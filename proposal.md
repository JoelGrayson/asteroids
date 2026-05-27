## **Title of your project**

Asteroids

## **Team members**

Joel Grayson, Sebastian Hazlett

## **Project goals**

Short description of project idea and end goal you are aiming for. Include stretch goals and/or fallback position to indicate how you might fine-tune the project scope if needed.

To recreate the 1979 Atari asteroids game, using a custom-written vector graphics library to recreate the original vector display charm.  
Stretch goal: leaderboard scoreboard after you die that you enter your name, nice explosion when asteroid

## **Resources, budget**

Itemize components/materials you will need and plan for acquiring them. We can reimburse cost of parts up to $20 per person.

* $50 14” [Monitor](https://www.amazon.com/WUAWE-Portable-Monitor-Kickstand-Speakers%EF%BC%8C/dp/B0FDGD6KH2/ref=sr_1_8?crid=QY7KCUCU4XJ3&dib=eyJ2IjoiMSJ9.mxicnnwk8xPF7XHzWjaNBI2DatN4O5fYIysWORueOkczRVSj9JJMnluvp1VG8OOCFmizd0pIFrwbzib0wPojF6SL0QmcNtFiE6n_fDskeUPr9wOLbl2fwpPxIWDiyAx2Ung-tGvxKS1fl-MaAivNXZiOufolXAtmBwLOPVcjMMQ179pvnDEG53ppyFqLoY1s1VkUp1loOAoEUC3HYtxrBk-c-GKnwQVWTn0kAxL0ERE.bSYu0VcfZF8C731qnLXXJXcIIk3Xu8iHK8wPfMbbwjY&dib_tag=se&keywords=14%2Binch%2Bhdmi%2Bmonitor&qid=1779157062&sprefix=14%2Binch%2Bhmonitor%2Caps%2C261&sr=8-8&th=1)  
* $10 [6 white buttons](https://www.amazon.com/gp/product/B07XYRD4LZ/ref=ox_sc_act_title_1?smid=A1THAZDOWP300U&th=1)  
* $7 [4 speakers](https://www.amazon.com/gp/product/B0D7S8FFKY/ref=ox_sc_act_title_2?smid=A2OEGRG2ZM4R5C&psc=1)  
* Mango pis  
* Wood  
* Power management: adapter/surge protector  
* Mini-HDMI display cable  
* SD card

\=$67  
99\*4=396  
67\*6=402

- Discuss about sale price, sale audience later.

## **Tasks, member responsibilities**

Each team member should be responsible for a clearly identifiable part of the project.  
Joel:  
– line interpolation  
– asteroid, saucer, bullet graphics  
– game logic (score tracking, 3 lives, saucer)  
Sebastian:  
– leaderboard  
– custom trig library for fast calculations  
– collision detection  
– audio  
– SD card persistent memory

## **Schedule, midpoint milestones**

Sketch a timeline for steady progress across full project period Order tasks so as to resolve unknowns/risks sooner than later What are the one-week goals you will complete by next lab meeting?

|  | Joel | Sebastian |
| :---- | :---- | :---- |
| 5/26 Tue | Rocket graphics (rendered) |  |
| 5/27 Wed | Update position of asteroids/rocket using vx/y | I2S audio, sound conversions playSound(BULLET) |
| 5/28 Thu | 5 buttons can be pressed Buttons | Rocket that can explode Buttons |
| 5/29 Fri | Rocket can shoot bullets  —bullets should despawn off screen | Asteroid spawning/respawning |
| 5/30 Sat | Rocket can rotate \+ move | Anything unfinished from last week |
| 5/31 Sun | Bullets can hit asteroid and destroy it | Saucer rendered Saucer shoots at you |
| 6/1 Mon | Score | Leaderboard |
| 6/2 Tue Seb PWR | Multiple lives ORDER ALL PARTS FOR MANUFACTURING | Game over/restart screen |
| 6/3 Wed | Asteroids turns into multiple asteroids  | Start persistent memory |
| 6/4 Thu | Nothing \- study for 221 exam | Try to finish persistent memory |
| 6/5 Fri Joel exam | Nothing \- 221 exam | Saucer moves around and can die |
| 6/6 Sat | FINISHED WITH LOGIC BEFORE TODAY Go to Home Depot to get wood | Help Joel finish |
| 6/7 Sun | Nice packaging and presentation Manufacture a few | Nice packaging and presentation Manufacture |
| 6/8 Mon |  Selling | Sebastian Selling |
| 6/9 Tue | Selling |  |
| 6/10 Wed Seb exam | Presentation to class |  |

Selling

* Make 10\. Price at $130. Costs $97 to make.  
* $15 warranty+support, say up front that mango pi is $30, so we can pay if something goes horribly wrong we’ll just fix it.  
* Market to  
  * Chris Gregg  
  * Prof. Horowitz  
  * Prof. Ron Fedkiw  
  * White Plaza with a stand

Week 9:  
– audio, sound replay with I2S  
– Prototype collision detection  
Week 10/Finals:  
– SD card persistent memory  
– Perfectly functional/extended collision detection

Loop over asteroid:

* Check if touching a bullet, rocket, or saucer

## **Additional resources, issues**

Are there books/code/tools that you need access to? What are the possible risks to overcome? What are you concerned about? How can we help you succeed?
