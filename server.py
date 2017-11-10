from modules import DNSListener
from modules import AgentControllerCLI
import threading



banner = """
    ____  _   _______       ____                 _      __ 
   / __ \/ | / / ___/      / __ \___  __________(_)____/ /_
  / / / /  |/ /\__ \______/ /_/ / _ \/ ___/ ___/ / ___/ __/
 / /_/ / /|  /___/ /_____/ ____/  __/ /  (__  ) (__  ) /_  
/_____/_/ |_//____/     /_/    \___/_/  /____/_/____/\__/  
                                                           
"""
host = "0.0.0.0"
port = 53

print banner


DNSObject = DNSListener.DNSListener(host,port)
commandInputs = DNSListener.Input().cmdloop()