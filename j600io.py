from migen import *
from litex.soc.interconnect import wishbone

class J600IO(Module):
	def __init__(self, u600, u601, u604, u605, u608):
		self.bus = bus = wishbone.Interface()

		regbufs = {
			"u600" : Signal(8),
			"u601" : Signal(8),
			"u604" : Signal(8),
			"u605" : Signal(8),
		}

		self.sync += [
			# DO NOT FORGET TO ACK <= '0' !!!!!!!! OTHERWISE THE CPU HANGS
			bus.ack.eq(0),
			If(bus.cyc & bus.stb & ~bus.ack,
				If(bus.we,
					If((bus.adr & 0x7) == 0,
						regbufs["u600"].eq(bus.dat_w & 0xFF),
					).Elif((bus.adr & 0x7) == 1,
						regbufs["u601"].eq(bus.dat_w & 0xFF),
					).Elif((bus.adr & 0x7) == 2,
						regbufs["u604"].eq(bus.dat_w & 0xFF),
					).Elif((bus.adr & 0x7) == 3,
						regbufs["u605"].eq(bus.dat_w & 0xFF),
					)
				).Else(
					If((bus.adr & 0x7) == 0,
						bus.dat_r.eq(regbufs["u600"]),
					).Elif((bus.adr & 0x7) == 1,
						bus.dat_r.eq(regbufs["u601"]),
					).Elif((bus.adr & 0x7) == 2,
						bus.dat_r.eq(regbufs["u604"]),
					).Elif((bus.adr & 0x7) == 3,
						bus.dat_r.eq(regbufs["u605"]),
					)
				),
				bus.ack.eq(1)
			),
		]

		self.comb += [
			u600.eq(regbufs["u600"]),
			u601.eq(regbufs["u601"]),
			u604.eq(regbufs["u604"]),
			u605.eq(regbufs["u605"])
		]