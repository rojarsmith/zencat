import os
from pyhawkag.HawkAgConfig import HawkAgConfig

def test_HawkAgConfig():
    print(os.getcwd())
    hawkag_config = HawkAgConfig()
    hawkag_config.load_config()
    hawkag_config.domain()
    assert hawkag_config.domain() == "https://hawkbit.bitdove.net"
