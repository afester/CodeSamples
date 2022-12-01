
import sys
from pymodbus.client.sync import ModbusTcpClient

INVERTER_IP = '192.168.178.46'
INVERTER_PORT = 1502
INVERTER_CONNECT_TIMEOUT = 3

# Register addresses
C_SunSpec_ID     = 40000
C_SunSpec_DID    = 40002
C_SunSpec_Length = 40003
C_Manufacturer   = 40004
C_Model          = 40020
C_Version        = 40044
C_SerialNumber   = 40052
C_DeviceAddress  = 40068

I_AC_VoltageAN   = 40079
I_AC_VoltageBN   = 40080
I_AC_VoltageCN   = 40081
I_AC_Voltage_SF  = 40082

I_AC_Power       = 40083
I_AC_Power_SF    = 40084

I_AC_Frequency   = 40085
I_AC_Frequency_SF= 40086

I_AC_VA = 40087
I_AC_VA_SF = 40088
I_AC_VAR = 40089
I_AC_VAR_SF = 40090
I_AC_PF = 40091
I_AC_PF_SF = 40092

I_AC_Energy_WH   = 40093
I_AC_Energy_WH_SF= 40095

I_Temp_Sink      = 40103
I_Temp_Sink_SF   = 40106

I_Status         = 40107
I_Status_Vendor  = 40108


# Works:
# $mbpoll -r 1 -c 2 -p 1502 192.168.178.46

# $ mbpoll -1 -t 4:hex -r 40005 -c 16 -p 1502 192.168.178.46
# ...
# [40005]: 	0x536F   So
# [40006]: 	0x6C61   la
# [40007]: 	0x7245   rE
# [40008]: 	0x6467   dg
# [40009]: 	0x6520   e
# [40010]: 	0x0000


def readString(client, addr, length):
    result = ""
    data = client.read_holding_registers(addr, length, unit=1)
    for idx in range(0, len(data.registers)):
        val = data.getRegister(idx)
        result = result + chr(val >> 8) + chr(val & 0xff)

    return result


def readUINT16(client, addr):
    data = client.read_holding_registers(addr, 1, unit=1)
    result = data.getRegister(0)
    return result

def twos_comp(val, bits):
    """compute the 2's complement of int value val"""
    if (val & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
        val = val - (1 << bits)        # compute negative value
    return val                         # return positive value as is

def readINT16(client, addr):
    data = client.read_holding_registers(addr, 1, unit=1)
    result = data.getRegister(0)
    return twos_comp(result, 16)


def readUINT32(client, addr):
    data = client.read_holding_registers(addr, 2, unit=1)
    result = data.getRegister(1) + (data.getRegister(0) << 16)
    return result


def readFloat(client, addrValue, addrScale):
    resultVal = readUINT16(client, addrValue)
    resultScale = readINT16(client, addrScale)
    return resultVal * pow(10, resultScale)


def readFloat2(client, addrValue, addrScale):
    resultVal = readUINT32(client, addrValue)
    resultScale = readINT16(client, addrScale)
    return resultVal * pow(10, resultScale)


def status2String(status):
    states = {1: "Off",
              2: "Sleeping (auto-shutdown) – Night mode",
              3: "Grid Monitoring/wake-up",
              4: "Inverter is ON and producing power",
              5: "Production (curtailed)",
              6: "Shutting down",
              7: "Fault",
              8: "Maintenance/setup"
              }

    result = states.get(status)
    return result if result is not None else "Unknown"


def main(args):
    client = ModbusTcpClient(INVERTER_IP, INVERTER_PORT, timeout=INVERTER_CONNECT_TIMEOUT)
    try:
        print("Connecting ...")
        if not client.connect():
            print("Could not connect to modbus device: {}".format(client.getErrorMessage()))
            return

        result = readUINT32(client, C_SunSpec_ID)
        print('"0x{:08x}"'.format(result))
        result = readUINT16(client, C_SunSpec_DID)
        print('"0x{:04x}"'.format(result))
        # result = readUINT16(client, C_SunSpec_Length)
        # print('"{}"'.format(result))
        #
        # result = readString(client, C_Manufacturer, 16)
        # print('"{}"'.format(result))
        # result = readString(client, C_Model, 16)
        # print('"{}"'.format(result))
        # result = readString(client, C_Version, 8)
        # print('"{}"'.format(result))
        # result = readString(client, C_SerialNumber, 16)
        # print('"{}"'.format(result))
        # result = readUINT16(client, C_DeviceAddress)
        # print('"{}"'.format(result))

        result = readFloat(client, I_AC_Frequency, I_AC_Frequency_SF)
        print('"{} Hz"'.format(result))
        result = readFloat(client, I_AC_VoltageAN, I_AC_Voltage_SF)
        print('"{} V"'.format(result))
        result = readFloat(client, I_AC_VoltageBN, I_AC_Voltage_SF)
        print('"{} V"'.format(result))
        result = readFloat(client, I_AC_VoltageCN, I_AC_Voltage_SF)
        print('"{} V"'.format(result))

        # Current production (????)
        result = readFloat(client, I_AC_Power, I_AC_Power_SF)
        print('"{} W"'.format(result))
        result = readFloat(client, I_AC_VA, I_AC_VA_SF)
        print('"{}"'.format(result))
        result = readFloat(client, I_AC_VAR, I_AC_VAR_SF)
        print('"{}"'.format(result))
        result = readFloat(client, I_AC_PF, I_AC_PF_SF)
        print('"{}"'.format(result))

        result = readFloat(client, I_Temp_Sink, I_Temp_Sink_SF)
        print('"{} °C"'.format(result))
        result = readFloat2(client, I_AC_Energy_WH, I_AC_Energy_WH_SF)
        print('"{} kWh"'.format(result / 1000))

        result = readUINT16(client, I_Status)
        print('\"{}\"'.format(status2String(result)))
        # result = readUINT16(client, I_Status_Vendor)
        # print(result)

        # First Meter
        result = readString(client, 40139, 16)
        print('"{}"'.format(result))
        result = readFloat(client, 40196, 40203)
        print('"{} V"'.format(result))

        result = readFloat2(client, 40226, 40242)
        print('"{} kWh"'.format(result/1000))
        result = readFloat2(client, 40234, 40242)
        print('"{} kWh"'.format(result/1000))


    finally:
        print("Closing connection.")
        client.close()


if __name__ == '__main__':
    main(sys.argv)
