from migen import *
from litex.soc.interconnect import wishbone
from math import exp, sin

# https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/

class HeartbeatPWM(Module):
	def __init__(self, freq_div, out=Signal()):
		self.counter = counter = Signal(max=255*freq_div)
		self.out = out
		self.value = value = Signal(max=255)
		value_buf = Signal(max=255)

		self.sync += [
			If(counter == 0,
				value_buf.eq(value)
			),
			If(counter == 255*freq_div,
				counter.eq(0)
			).Else(
				counter.eq(counter+1)
			)
		]

		self.comb += [ 
			self.out.eq(counter < value_buf*freq_div)
		]

def heartbeatPwmTest(dut):
	for i in range(5000):
		print("{} {} {}".format((yield dut.counter), (yield dut.out), (yield dut.out)))
		yield

def heartbeatPwmTestbench():
	pwm = Signal()
	dut = HeartbeatPWM(10, out=pwm)
	run_simulation(dut, heartbeatPwmTest(dut), vcd_name="heartbeatpwm.vcd")

def animationFunc(t):
	return int(256*(exp(sin(t)) - 1)/(exp(1) - 1))

class Heartbeat(Module):
	def __init__(self, sys_clk_freq, heart_freq, out=Signal()):
		lutSize = 256
		lutExpSin = Array([animationFunc(i*3.14/lutSize/2) for i in range(0,lutSize)])
		#print(lutExpSin)
		#exit()
		posInAnimation = Signal(max=lutSize)

		self.out = out

		fsm = FSM(reset_state="FORWARD")
		self.submodules += fsm

		self.pwm = pwm = HeartbeatPWM(10, out)
		self.submodules += pwm

		subcounterPeriod = int(sys_clk_freq/heart_freq/lutSize)
		subcounter = Signal(max=subcounterPeriod)

		fsm.act("FORWARD",
			If(posInAnimation == lutSize - 1,
				NextState("BACKWARDS")
			)
		)

		fsm.act("BACKWARDS",
			If(posInAnimation == 0,
				NextState("FORWARD")
			)
		)

		self.sync += [
			If(subcounter == subcounterPeriod,
				subcounter.eq(0),
				If(fsm.ongoing("FORWARD"),
					posInAnimation.eq(posInAnimation+1)
				).Else(
					posInAnimation.eq(posInAnimation-1)
				)
			).Else(
				subcounter.eq(subcounter+1)
			)
		]

		self.comb += [
			pwm.value.eq(lutExpSin[posInAnimation])
		]

def heartbeatTest(dut):
	for i in range(25000):
		print("{} {}".format((yield dut.pwm.value), (yield dut.out)))
		yield

def heartbeatTestbench():
	pwm = Signal()
	dut = Heartbeat(5000, 0.2, out=pwm)
	run_simulation(dut, heartbeatTest(dut), vcd_name="heartbeat.vcd")

if __name__ == "__main__":
    heartbeatPwmTestbench()
    heartbeatTestbench()