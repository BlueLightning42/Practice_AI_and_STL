# calculator for doing (simple) engineering calculations taking into account inches
# basically just a wrapper for pint with support for feet and inches as well as numpy arrays
# examples:
# 1' 2 1/4" + 0' 1 1/2" 		= 15.75 in
# 12'4" / 2 1/2 + 5" * 3 1/2 	= 76.7 in
# [1,2,3,4,5] ' * 2" 			= [ 24  48  72  96 120] in²
# 500 1/2' * 9 3/4 				= 58.5585 kin



from pint import UnitRegistry
import numpy
import re
ureg = UnitRegistry()
Q_ = ureg.Quantity

running = True

ureg.default_format = '~P'
numpy.warnings.filterwarnings('ignore')

_MIXED_FORMAT = re.compile(r"""
    ([-+]?)           # an optional sign, then
    (\d+)?            # integer part
    \s*?              # whitespace
    (\d+)             # numerator
    /(\d+)            # denominator
""", re.VERBOSE)

_FEET_AND_INCHES = re.compile(r"""
    (?P<feet>(\d+(\.\d*)?|(\d+)?\.\d+)(e[-+]?\d+)?) # a number
	'\s*? 									# feet
	(?P<inches>(\d+(\.\d*)?|(\d+)?\.\d+)(e[-+]?\d+)?) # a number
	"		 								# inch
""", re.VERBOSE)

_INCH = re.compile(r'(?P<inches>(\d+(\.\d*)?|(\d+)?\.\d+)(e[-+]?\d+)?)\s?(")')
_FOOT = re.compile(r"(?P<feet>(\d+(\.\d*)?|(\d+)?\.\d+)(e[-+]?\d+)?)\s?(')")

def mix_fractions(s):
	return _MIXED_FORMAT.sub(lambda m: str(eval(m.group().replace(" ", "+ "))), s)

def fix_feet_and_inches(s):
	s = _FEET_AND_INCHES.sub(lambda m: " ({}* 12* ureg.inch + {}* ureg.inch)".format(m.group("feet"), m.group("inches")), s)
	s = _INCH.sub(lambda m: "({!s}* ureg.inch)".format(m.group("inches")), s)
	s = _FOOT.sub(lambda m: "({!s}*ureg.inch*12)".format(m.group("feet")), s)
	s = s.replace('"', "* ureg.inch")
	s = s.replace("'", "* ureg.inch*12")
	return s;

while running:
	try:
		user_imput = input("\n= ").strip()
		if user_imput.lower() in ["stop", "end", "quit"]:
			running = False
			continue

		# deals with mixed fractions like 1 1/2
		user_imput = mix_fractions(user_imput)

		# deals with ' and " postfixes
		user_imput = fix_feet_and_inches(user_imput)

		# remove + if last item in formula
		if user_imput[-1] is '+':
			user_imput = user_imput[:-2]
		
		print("=", user_imput.replace("* ureg.inch", 'in'))


		if " to " in user_imput:
			(l,r) = user_imput.split(" to ")
			formula = "Q_({0}).to(\"{1}\")".format(l,r)
		else:
			formula = "Q_({})".format(user_imput)
		#print(formula)
		answer = "no answer"
		
		answer = eval(formula)
		print("=", Q_(answer).to_compact())
	except Exception as e:
		print(e)
