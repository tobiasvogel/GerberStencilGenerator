# GerberStencilGenerator
Simple and intuitive Gerber RS-274X editor for easily creating PCB Stencils
---
### Introduction

As a Hobbyist n the DIY-/Maker Community designing small PCBs I usually order my PCBs from a Manufacturer together with a accompanying Solder Past Stencil for SMD Components Soldering.

Some more advanced Editors have special Functions to generate a custom Stencil Gerber File, but many will only leave you with the sole Option to export the Solder Paste Layer and use it as the Stencil File "as is". 
Often that will work just fine, but sometimes having the ability to customize the File a bit more would come in quite handy.

Some Manufacturers allow size-adjustments or corner-modifications of the Stencil Shapes - however, mostly with one setting to be applied to all Shapes equally.

If you are like me, you just want that little extra flexibility of adjusting each group of Shapes individually. This lead to the development of this very Program.

As a final note let me clearly say, that it is far from being perfect or even completed - it is a Work-In-Progress with a lot left to do, but it is a start at least. 

### What it is and what it does
The GerberStencilGenerator will take any existing RS-274X Gerber File, search for the Shapes being used.
Each Shape can be resized and where applicable, corners can be rounded. 

Once you are satisfied with the modifications you made, the Program will output a new Gerber File reflecting your changes accordingly and optionally a ".PNG image.

Please note that GerberStencilGenerator intentionally will not overwrite your original File, as by the nature of the Gerber Format introducing rounded corners requires replacing some simple shapes like a rectangle by a group of others to accurately represent the desired shapes. Therefore once the output file is written, the group of shapes cannot be changed back into a single shape as easily and retaining the original File is therefore desired and is not a Bug of the Program.

### What it isn't and limitations it has
The program does is not a dedicated Gerber File Generator by itself. It will only modify already existing Gerber Files, you still need a PCB-Tool to generate a Gerber File in the first place. 
