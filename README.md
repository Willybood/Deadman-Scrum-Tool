# The Deadman Scrum Tool
This project is for a deadman's switch that is passed around to different team members during a stand up. The user pushes a button on the device, then after 90 seconds, it begins buzzing to tell the user to end.
## About the Deadman
While introducing Scrum to a new team, we quickly had issues with team members wanting to speak over each other during stand ups and extend the meeting beyond the 15 minutes. During one of the early retrospectives someone suggested a handheld device that lets people know if they've been talking for too long, and using left over parts and a public 3D printer I began prototyping. After introducing it, the stand ups have been smoother.
## How does it work
1. Turn on the power switch on the bottom. The device will boot up and a green light will come on.
2. Grip the device in your hand so the large switch is held down. The light will begin to slowly flash red. During which you tell the rest of the team the standard stand up procedure of what you've worked on, what you'll be working on, and what's blocking you.
3. The flashing will gradually increase up until your 90 seconds are up, after which it will begin to play 'Shave and a haircut' until the switch is released.
4. After you're finished, release the grip and hand it to the next person.
5. And after the team is finished, turn off the switch on the bottom and the light will go out.
## Software
The device runs on an Adafruit Trinket. The .ino file is available to upload [here](https://github.com/Willybood/Deadman-Scrum-Tool/tree/master/deadman).
## Hardware
The device consists of a set of electronic components inserted into three 3D printed parts.
### 3D printed parts
The device consists of three prints:
* [caseHalfLeft.stl](https://github.com/Willybood/Deadman-Scrum-Tool/blob/ReadmeInProgress/caseHalfLeft.stl) and [caseHalfRight.stl](https://github.com/Willybood/Deadman-Scrum-Tool/blob/ReadmeInProgress/caseHalfRight.stl). These are printed and then can be pushed in place with no glue needed.
* Holder **Push this file and update**. This is the component that is gripped to close the switch. After printing it, it can be placed into the holes on the side of the case.
### Components
* [Adafruit Trinket](https://www.amazon.co.uk/gp/product/B00M40XTI4/ref=oh_aui_detailpage_o05_s00?ie=UTF8&psc=1)
* [Hinge lever switch](https://www.amazon.co.uk/gp/product/B00M1ZTO2C/ref=oh_aui_detailpage_o04_s01?ie=UTF8&psc=1)
* [AAA battery case holder](https://www.amazon.co.uk/gp/product/B00H8T6J3S/ref=oh_aui_detailpage_o04_s00?ie=UTF8&psc=1)
* [AAA form factor li-ion battery](https://www.amazon.co.uk/gp/product/B016CT546C/ref=oh_aui_detailpage_o04_s00?ie=UTF8&psc=1)
* [Two position switch](https://www.amazon.co.uk/gp/product/B007QAJMHO/ref=oh_aui_detailpage_o03_s00?ie=UTF8&psc=1)
* [Low profile piezo sounder](https://www.maplin.co.uk/p/30v-very-low-profile-piezo-sounder-4mm-ku57m)
* [RGB LED board](https://www.amazon.co.uk/kwmobile-RGB-LED-module-Arduino/dp/B01N248IOR/ref=sr_1_22?ie=UTF8&qid=1515148985&sr=8-22&keywords=RGB+LED+board), you can use a three color LED and a pair of resistors, but I had one of these lying around.

I used some [gorilla glue](https://www.amazon.co.uk/gp/product/B009NQQJFC/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1) to hold some of the components in place.
![Fritzing diagram](/Board.png?raw=true "Fritzing diagram")
<p align="center">*Fritzing diagram describing the components wiring*</p>
## Special thanks
* I'd like to thank my team for coming up with the idea and trying out my prototypes.
* One inspiration for this came from the [Scrumato](https://github.com/platisd/scrumtato) stand up tool, which I would have built instead if only I had the components for it lying around around. It looks like a good tool, and I'm sure it would have worked too.
