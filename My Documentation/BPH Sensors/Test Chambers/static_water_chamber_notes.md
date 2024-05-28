# How to Create a Static Water Test Chamber
## Starling Medical
### Ibrahim Al-Akash

# Static Water Test Chamber Notes

## Overview
The static water test chamber will be used to hold at least 100g of water while keeping the magnetometer sensor circuit safe from any leakage. To create a static water test chamber, first design the appropriate chamber geometry for the elastomer being tested, 3D print the design, assemble the testing apparatus, and inspect and iterate the waterproofing of the chamber.


## Design
The test chamber cross-sectional area should ideally be equal to the surface area of the elastomer being used in the experiment. This ensures that 100% of the mass of the water is being imparted onto the elastomer and that none of the load is transferred to the chamber itself. By meeting this criteria, the pressure on the elastomer is maximized, resulting in an increased sensitivity and a higher reslution sensing capability. A design with a larger cross-sectional area will require a taller chamber since a larger mass of water will be needed to impart enough force and pressure to compress the elastomer. Remember the equation $Pressure=\frac{Force}{Area}$.

## 3D Printing
1. Design the chamber in SolidWorks.
2. Save SolidWorks design as an `.STL` file and import to Chitubox software.
3. In Chitubox software, apply a tilt if possible to the model (for example rotate around z-axis by 45 degrees) and the add supports to model.
4. Ensure the correct resin profile is selected so that the profile matches the resin you will be printing with. Ensure the correct printer is selected in the Chitubox settings.
5. Slice the model and inspect it for any artifacts or errors. If there are errors, try rotating along a different axis and reapplying the supports.
6. Export the slice which will be a `.CTB` file to the directory of your choosing.
7. Upload the exported slice file to a USB flashdrive to send to the 3D printer.
8. Ensure the 3D printer has enough resin to print the parts.
9. Begin the print on the 3D printer.
10. When the 3D print is completed, carefully remove the parts from the plate by using the metal spatula. If the part is stuck, spray a little isopropyl alcohol on it and try again.
11. There should be 3 containers next to the 3D printers labeled `99% Isopropyl Alcohol`, `99% Isopropyl Alcohol (Contaminated)`, and `Diluted Green Wash`. Thoroughly clean the printed parts in each bucket in this order.
12. Set the parts next to the fan and wait ~5 minutes to dry.
13. Place the parts in the water cage in the container at the curing station. Remove the curing plate from the machine and place the water container in its place. Close the lid on it. Make sure the curing machine is set to rotation mode, not light mode. Set the time to 5:00 (6:00 if there are larger parts) and let the machine rinse the parts.
14. Once the rinse step is completed, set the parts next to the fan and wait 10 to 15 minutes until they are dry.
15. Once the parts are finished, place the curing plate on the curing machine. Ensure the magnets are aligned correctly so the plate is flush against the base. Ensure the curing machine is set to light mode. Set the time to 5:00.
16. Once the curing is completed, carefully remove the supports from the parts using pliers.

## Assembly
Thoroughly coat the outer surface of the test chamber with the waterproofing silicone adhesive paste. Let it dry for 10 to 30 minutes.

After assembling the elastomer-magnetometer apparatus according to the document titled `elastomer_creation_procedure.md` or `elastomer_creation_procedure.pdf`, apply superglue to the bottom surface of the test chamber and press the elastomer-magnetometer apparatus up into the chamber orifice. The elastomer should NOT be superglued, it should freely move within the chamber and should be snugly fit. Only the magnetometer sensor module PCB should be secured to the chamber. *This is a critical step I realized while assembling my last chamber for the Starling Medical Live Demo Showcase. If the elastomer is superglued inside the chamber, it will not compress properly and will result in faulty sensor readings.* Let the glue dry for 10 to 30 minutes.

## Waterproofing
Before attempting to perform the experiment, you must ensure that the chamber is adequately waterproofed to protect the sensor electronics. To do this, go to the sink and carefully begin adding water. After a decent amount of water is added, wait for at least 2 minutes and inspect the assembled testing apparatus for any leakage. Take note of any spots which are leaking and apply more waterproofing silicone adhesive paste to those areas.