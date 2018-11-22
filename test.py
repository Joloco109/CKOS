#/bin/python

import krpc

connect = krpc.connect()
vessel = connect.space_center.active_vessel

for i in range(5,8):
    print("Stage:{}".format(i))
    for j in vessel.resources_in_decouple_stage(i).all:
        print("{}={}".format(j.name, j.amount))

    print(" and SolidFuel={}".format(vessel.resources_in_decouple_stage(i).amount("SolidFuel")))
