from jproperties import Properties

class HawkAgConfig:
    def __init__(self, domain=None):
        self._domain = domain

    def domain(self, domain):
        self._domain = domain
    
    def domain(self):
        return self._domain

    def load_config(self):
        # return self.domain()
        # Load config
        with open("config.properties", "r+b") as f:
            p = Properties()
            p.load(f, "utf-8")
        
            self._domain = p.properties.get("hawkbit.domain")
            # HawkAgConfig.domain()
            # HawkAgConfig.domain = p.properties.get("hawkbit.domain")
            # if hawkbit_domain.startswith("https://"):
            #     hawkbit_domain = hawkbit_domain[len("https://"):]
            # hawkbit_username = p.properties.get("hawkbit.username")
            # hawkbit_password = p.properties.get("hawkbit.password")
            # hawkbit_gatewaysecuritytoken = p.properties.get("hawkbit.gateway-security-token")
            # global hawkbit_devicetargetid 
            # hawkbit_devicetargetid = p.properties.get("hawkbit.device-target-id")
            # hawkbit_devicesecuritytoken = p.properties.get("hawkbit.device-security-token")