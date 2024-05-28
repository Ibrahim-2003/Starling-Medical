# How to Design an Elastomer for Magnetic BPH Sensor
## Starling Medical
### Ibrahim Al-Akash

# Elastomer Design Notes

## Overview
To design a proper elastomer for the magnetometer BPH sensor, you must first determine the geometry you want to test, model the elastomer, and model the elastomer mold.

## Elastomer Geometry
Elastomer geometry is important in determining the magnetometer sensitivity. Some basic principles to keep in mind are the following:

- Shorter elastomers are more malleable, however, the magnet has a smaller range of distances it can move. This limits the range of masses the sensor can detect.

- Thicker elastomers enable the magnet to move a larger range of distances, however, they are more rigid. This limits the sensitivity of the magnet, which translates to a lower resolution of masses it can detect (ex: $\pm$ 0.01g vs. $\pm$ 5g).

- Elastomers with a larger surface area in contact with the water will require more water to deform. An equation that is important to keep in mind is $Pressure=\frac{Force}{Area}$. Force is the mass of the water and area is the area of the elastomer in contact with the water. If the area is too large, a very large force will be required to supply adequate pressure to compress the elastomer, which means more water must be added.

- Elastomers with high durometer rating will be harder to compress, and require either more water or a smaller area in contact with the water.

- Elastomers with larger magnets will enable the sensor to read a wider range of masses, however this comes at the price of sensitivity. This limits the resolution of the sensor. This is due to the fact that stronger magnets can reach a farther distance from the sensor before their B-field begins to drop off significantly. The B-field of a weaker magnet drops off very rapidly when moving only a small distance. This makes it more sensitive, however it comes at the cost of range.

- The researchers who wrote the paper on this technology recommend embedding the magnet in the top of the elastome to prevent hysterisis and creep from infecting the magnet and introducing noise to the data over time.

## Modeling the Elastomer
Prior to modeling the elastomer mold, I recommend modeling the elastomer as you would like it to look when the silicone is fully cured. Make sure you do not forget to include a notch so that the magnet can be flush with the top surface of the elastomer. By modeling the elastomer before the mold, you can easily calculate dimensions and evaluate mass and other properties to better inform your design decisions.

## Modeling the Elastomer Mold
For some elastomer designs, you can make a 1-piece mold. These are elastomers in which only 1 surface has an indentation (the surface containing the magnet). If you are making an elastomer in which both the top and bottom surfaces have indentations or notches, you need to make a 2-piece mold.

### 1-Piece Mold
1. Measure the elastomer dimensions using the `Evaluate` tool in SolidWorks.
2. Make a block with some leeway for the dimensions of the elastomer. For example, if you are making an elastomer that is $5\times5\times5mm^3$, make the block $10\times10\times10mm^3$.
This makes it easier to work with the 3D print and prevents you from accidentally cracking the printed mold due to it being to thin and fragile.
3. Make an assembly with the block and the elastomer model.
4. Create a mold into the block using the elastomer model with the notch facing inward into the block and the flat surface of the elastomer flush with the flat surface of the block.

### 2-Piece Mold
1. Measure the elastomer dimensions using the `Evaluate` tool in SolidWorks.
2. Make the bottom mold, by creating a block and then manually drawing sketches and extrude cutting.
3. To Make the top mold, I recommend making a flat part that is flush with the top surface of the bottom mold, and then extruding out the piece which will make the notch for the magnet. This makes it easier when manufacturing since you can pour the silicone into the bottom mold and then press the top mold into the bottom one.