import math
import numpy as np

# 'Material Name': [Cost/lb, in^3/lb] 
materials = {'VytaFlex 30': [27.91/2, 27.3],
            'EcoFlex 00-30': [35.58/2, 26.0],
            'ReoFlex 30': [27.90/2, 27.5],
            'VytaFlex 20': [27.18/2, 27.7],
            'EcoFlex 00-20': [35.67/2, 27.5],
            'DragonSkin 20': [37.05/2, 25.6],
            'ReoFlex 20': [31.91/2, 27.3]}

materials = {'Task 21': [66.59/3, 25.9]}

k = 1/16387.064069264 # constant for 1 cu in / 16387 cu mm
suction_cup_inside_volume = 48788.25 # cu mm
attachment_module_soft_volume = 16791.20 # cu mm
attachment_module_backplate_v4_volume = 50198.82 # cu mm
attachment_module_backplate_v3_volume = 96900.34 # cu mm
attachment_module_backplate_v2_volume = 94903.09 # cu mm
attachment_module_clip_volume = 5107.39 # cu mm
formable_wire_casing_volume = 20333.59 # cu mm
suction_cup_inside_redirectors_volume = 75868.29 # cu mm
hard_connector_volume =  5107.39 # cu mm
magnetic_exterior = 47958.66 # cu mm
magnetic_backplate = 116163.95 # cu mm


def calc(materials, k, volume, individual_report, summary):
    prices = []
    mats = []

    for material in materials.items():
        cost_per_lb = material[1][0]
        specific_volume = material[1][1]
        unit_cost = cost_per_lb / specific_volume * k * volume
        prices.append(unit_cost)
        if individual_report:
            print(f'Material: {material[0]}\nCost per lb: ${cost_per_lb:.2f}\nSpecific Volume: {specific_volume} cu in/lb\nUnit Cost: ${unit_cost:0.2f}\n\n')
        mats.append(material[0])
        min_price = np.min(prices)

    min_material = mats[prices.index(min_price)]
    if summary:
        print(f'Cheapest Material: {min_material}, ${min_price:0.2f}\n')
    return min_price

print('Suction Cup Inside')
suction_cup_inside_volume_cost = calc(materials, k, 
                                    suction_cup_inside_volume - attachment_module_clip_volume, 
                                    individual_report = False, 
                                    summary = True)

print('Suction Cup Outside V2')
suction_cup_outside_v2_cost = calc(materials, k, 
                                    attachment_module_soft_volume + attachment_module_backplate_v3_volume + formable_wire_casing_volume, 
                                    individual_report = False, 
                                    summary = True)

print('Suction Cup Outside V3')
suction_cup_outside_v3_cost = calc(materials, k, 
                                    attachment_module_soft_volume + attachment_module_backplate_v4_volume + formable_wire_casing_volume, 
                                    individual_report = False, 
                                    summary = True)

print('Mechanical Clamp V1')
mechanical_clamp_v1_cost = calc(materials, k, 
                                    attachment_module_soft_volume + attachment_module_backplate_v2_volume, 
                                    individual_report = False, 
                                    summary = True)

# Volume Calculator for Mold/Cast
# pA = 1.15/1000
# pB = pA
# A = np.matrix([[1/pA, 1/pB], [10, -100]])
# b = np.array([14*14*8, 0])

# solution = np.linalg.solve(A, b)

# print(solution)

# Va = solution[0]/pA/1000
# Vb = solution[1]/pB/1000
# print(Va, Vb)
# print(1568 / 1000)

print('Suction Cup Inside V2')
mechanical_clamp_v1_cost = calc(materials, k, 
                                    suction_cup_inside_redirectors_volume, 
                                    individual_report = False, 
                                    summary = True)

print('Hard Bit')
hard_bit_cost = calc(materials, k,
                     hard_connector_volume,
                     individual_report=False,
                     summary=True)

print('Magnetic Attachment')
magnetic_attachment = calc(materials, k,
                           magnetic_backplate + magnetic_exterior,
                           individual_report=False,
                           summary=True)