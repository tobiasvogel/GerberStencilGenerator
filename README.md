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
The program isn't a dedicated Gerber File Generator by itself. It will only modify already existing Gerber Files, you still need a PCB-Tool to generate a Gerber File in the first place.
Another limitation (at least for now) is that GerberStencilGenerator can only edit so-called Flash-Apertures.
Due to historical reasons, the Gerber RS-274X Format only knows four basic types of basic shapes: circles, rectangles, obround and polygons, optinally rotated, with or without a hole in it.
These are also the only four shapes GerberStencilGenerator can actually edit. The Gerber Format itself also allows for what is called a Macro-Aperture, basically a custom shaped Aperture made from arbitrary combinations of the aforementioned shapes.
While GerberStencilGenerator will display these shapes it currently cannot edit those. Such shapes will remain untouched by GerberStencilGenerator. However, for most Applications this won't be a problem, as most (SMD-) Components will use either circle or rectangle shaped solder-pads anyway.

### Requirements
GerberStencilGenerator is written in C++ and relies on the Qt Framework.
This provides a reliable cross-platform compatibility and ensures, it runs likewise on Windows, Mac and Linux Systems.

Furthermore GerberStencilGenerator makes use of the [gerbv](http://gerbv.geda-project.org/) Gerber Editor program to display a preview image of the changes it makes. Although the preview-generation is optional and thus gerbv not required as a necessary dependency, being able to see your changes on-the-fly makes things a lot easier.

### Usage instructions
1. Create a RS-274X Gerber File just like you would in order to prepare a PCB-Ordering.
   You should preferably choose the dedicated Layers for Stencil creation. In EAGLE these are Layer **31 tCream** and/or Layer **32 bCream** respectively. In other CAD programs they might be named differently.
2. Open the File within the GerberStencilGenerator.
    Each group of Apertures will be listed in the column to the left.
