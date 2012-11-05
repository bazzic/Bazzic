<?php

/**
 * Implements hook_views_data_alter().
 */
function photobook_views_pre_render(&$view) {
  //($view);
}

/**
 * Implements hook_views_data().
 */
function photobook_views_data_alter(&$data) {
  $data['commerce_product']['price_total'] = array(
    'title' => t('Price Total'),
    'help' => t('The total of all product prices in the View result.'),
    'area' => array(
      'handler' => 'photobook_handler_area_price_total',
    ),
  );
}