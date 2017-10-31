#pragma once

/*! When set to access point mode, this will be the default SSID.  
 * NOTE: over-ride this by setting EsApConfigurator.setApDetails(...)
 */
#define AP_NAME            "EspApConfigurator"

/*! When set to access point mode, this will be the default passphrase.
 * NULL means open WiFi AP - probably inadvisable.
 * NOTE: over-ride this by setting EsApConfigurator.setApDetails(...)
 */
#define AP_PASS            "configure"

