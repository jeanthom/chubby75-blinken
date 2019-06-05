from migen import *
from litex.soc.interconnect import wishbone

class J600IO(Module):
	def __init__(self, u600, u601, u604, u605, u608):
		self.bus = bus = wishbone.Interface()

		self.sync += [
			# DO NOT FORGET TO ACK <= '0' !!!!!!!! OTHERWISE THE CPU HANGS
			bus.ack.eq(0),
			If(bus.cyc & bus.stb & ~bus.ack,
				If(bus.we,
					#If(bus.adr & 0x7 == 0,
						u600[0].eq(bus.dat_w[0]),
						u600[1].eq(bus.dat_w[0]),
						u600[2].eq(bus.dat_w[0]),
						u600[3].eq(bus.dat_w[0]),
						u600[4].eq(bus.dat_w[0]),
						u600[5].eq(bus.dat_w[0]),
						u600[6].eq(bus.dat_w[0]),
						u600[7].eq(bus.dat_w[0]),
						#u601.eq((bus.dat_w[0] >> 8) & 0xFF)
					#).Elif(bus.adr & 0x7 == 1,
						#u604.eq(bus.dat_w[0] & 0xFF),
						#u605.eq((bus.dat_w[0] >> 8) & 0xFF)
					#).Elif(bus.adr & 0x7 == 2,
						#u605.eq(bus.dat_w[0] & 0xFF),
					#)
				),
				bus.ack.eq(1)
			),
			#u600[0].eq(1),
			#u600[1].eq(1),
			#u600[2].eq(1),
			#u600[3].eq(1),
			#u600[4].eq(1),
			#u600[5].eq(1),
			#u600[6].eq(1),
			#u600[7].eq(1),
		]