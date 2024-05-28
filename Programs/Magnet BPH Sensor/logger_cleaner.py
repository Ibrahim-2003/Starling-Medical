# script -a -t0 filename
# screen /dev/cu.usbmodem11301 115200
# ctrl A + ctrl K to kill
# ctrl D to stop

switches = {"Unable to read XYZ data from the sensor.": "",
              "" : ",",
            "  " : ",",
            " " : ",",
            "[4C" : "",
              "[7C" : ",",
              "[8C" : "",
              "[6C" : "",
              "[5C" : "",
              "[24C" : ",",
              "[16C" : ",",
              "[79C" : ",",
              "[72C" : ",",
              "[48C" : ",",
              "[56C" : ",",
              "[40C" : ",",
              "[64C" : ",",
              "[40C" : ",",
              "[23;80H" : ",",
              "[32C": ",",
              "[15C": ",",
              "[21C": ",",
              "[39C": ",",
              "[31C": ",",
              "[47C": ",",
              "[55C": ",",
              "[63C": ",",
              "[71C": ",",
              "[23C": ",",
              '\n\n': '\n',
              ',,': ',',
              '\t': ','}

res = ""
FILEPATH = "/Users/jordanianjoker/Desktop/Starling-Medical/Programs/Magnet BPH Sensor/"
# FILEPATH = "/Users/jordanianjoker/Desktop/Starling-Medical/Programs/Differential Pressure BPH Sensor/"
FILENAME = "soft_test_no_load_test1.txt"
NEWFILE = "soft_no_load_test1"

with open(FILEPATH + FILENAME) as my_file:
    res = my_file.read()
for k,v in switches.items():
    res = res.replace(k,v)

print(res)


with open(FILEPATH + NEWFILE + ".csv",'w') as csv:
    csv.write(res)