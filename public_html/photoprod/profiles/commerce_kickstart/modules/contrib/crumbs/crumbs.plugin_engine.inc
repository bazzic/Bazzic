<?php


function _crumbs_load_plugin_engine() {
  list($plugins, $disabled_keys) = crumbs_get_plugins();
  $weights = _crumbs_load_weights();
  foreach ($disabled_keys as $key => $disabled) {
    if (!isset($weights[$key])) {
      $weights[$key] = FALSE;
    }
  }
  return new crumbs_PluginEngine($plugins, $weights);
}


function crumbs_get_plugins() {
  static $_plugins;
  static $_disabled_keys;
  if (!isset($_plugins)) {
    list($_plugins, $_disabled_keys) = _crumbs_load_plugins();
  }
  return array($_plugins, $_disabled_keys);
}


function _crumbs_load_plugins() {
  foreach (array(
    'crumbs', 'menu', 'forum', 'views_ui', 'og', 'clickpath', 'menu_position',
    'taxonomy', 'path', 'pathauto', 'entityreference', 'comment', 'blog',
  ) as $module) {
    if (module_exists($module)) {
      module_load_include('inc', 'crumbs', 'plugins/crumbs.'. $module);
    }
  }
  $plugins = array();
  $disabled_keys = array();
  $api = new crumbs_InjectedAPI_hookCrumbsPlugins($plugins, $disabled_keys);
  foreach (module_implements('crumbs_plugins') as $module) {
    $function = $module .'_crumbs_plugins';
    $api->setModule($module);
    $function($api);
  }
  return array($plugins, $disabled_keys);
}


function _crumbs_load_weights() {
  $weights = variable_get('crumbs_weights', array());
  asort($weights);
  if (!isset($weights['*'])) {
    $weights['*'] = count($weights);
  }
  return $weights;
}


function _crumbs_load_weights_1_x() {
  $weights_1_x = variable_get('crumbs', array());
  $weights_1_x = is_array($weights_1_x) ? $weights_1_x : array();
  $weights = array();
  $weight = 0;
  foreach ($weights_1_x as $key_1_x => $enabled) {
    // Replace ':' by '.'.
    $key = str_replace(':', '.', $key_1_x);
    if ($enabled) {
      $weights[$key] = $weight;
      ++ $weight;
    }
    else {
      $weights[$key] = FALSE;
    }
  }
  return $weights;
}


/**
 * Sort plugins by the order they need to be invoked,
 * and remove those that don't need to be invoked at all.
 */
function _crumbs_sort_plugins___DISABLED(array &$plugins, array $key_weights) {
  $plugin_weights = array();
  foreach ($plugins as $plugin_key => $plugin) {
    $fragments = explode('.', $plugin_key);
    $partial_key = array_shift($fragments);
    $weight = 0;
    if (isset($key_weights['*'])) {
      $weight = $key_weights['*'];
    }
    while (TRUE) {
      if (isset($key_weights[$partial_key .'.*'])) {
        $weight = $key_weights[$partial_key .'.*'];
      }
      if (empty($fragments)) break;
      $partial_key .= '.'. array_shift($fragments);
    }
    if (isset($key_weights[$plugin_key])) {
      $weight = $key_weights[$plugin_key];
    }
    $plugin_weights[$plugin_key] = $weight;
  }
  foreach ($key_weights as $key => $weight) {
    $fragments = explode('.', $key);
    $partial_key = array_shift($fragments);
    $plugin_key = NULL;
    while (TRUE) {
      if (isset($plugins[$partial_key])) {
        $plugin_key = $partial_key;
      }
      if (empty($fragments)) break;
      $partial_key .= '.'. array_shift($fragments);
    }
    if (isset($plugin_key)) {
      if ($plugin_weights[$plugin_key] === FALSE || $plugin_weights[$plugin_key] > $weight) {
        $plugin_weights[$plugin_key] = $weight;
      }
    }
  }
  // this works, because the keys are never numeric.
  // (each key contains a module name)
  array_multisort($plugin_weights, $plugins);
  foreach ($plugins as $plugin_key => $plugin) {
    if ($plugin_weights[$plugin_key] === FALSE) {
      unset($plugins[$plugin_key]);
    }
  }
}




