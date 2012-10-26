Overview
========
Payleap gateway support for Drupal Commerce module.

Summary
=======
Adds two payment methods:

 * Payleap direct - directly process a credit card request.

 * Payleap CardOnFile - Saves the credit card information before processing for
   later use.

Added a recurring payment option, available with the included commerce_sp_payleap
module. Credit card info saved with CardOnFile method aren't available for
subscription transactions since they're not implemented in Payleap API.

Notice recurring payment option will activate only if there is only one item in
the cart and it's a subscription (see commerce_sp_payleap_order_contains_recurrent_product())
for more info.

Methods
=======
Most implemented through payment forms.
To cancel a subscription, use commerce_sp_payleap_cancel_contract().
