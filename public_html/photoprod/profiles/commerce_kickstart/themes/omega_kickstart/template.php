<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 *
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */

/**
 * Preprocess variables for html.tpl.php
 *
 * @see system_elements()
 * @see html.tpl.php
 */
function omega_kickstart_preprocess_html(&$variables) {
  // Add conditional stylesheets for IE
  $theme_path = drupal_get_path('theme', 'omega_kickstart');
  drupal_add_css($theme_path . '/css/ie-lte-8.css', array('group' => CSS_THEME, 'weight' => 20, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css($theme_path . '/css/ie-lte-7.css', array('group' => CSS_THEME, 'weight' => 21, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
}
