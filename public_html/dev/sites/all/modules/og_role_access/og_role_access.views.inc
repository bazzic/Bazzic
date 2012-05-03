<?php
/**
* @file
* Implementation of hook_views_plugin().
*/
function og_role_access_views_plugins() {
  $plugins = array(
    'access' => array(
      'og_role_access' => array(
        'title' => t('OG group role'),
        'help' => t('Access will be granted to users with permissions based on the group role.'),
        'handler' => 'og_role_access_plugin_access_role',
        'uses options' => TRUE,
        'path' => drupal_get_path('module', 'og_role_access'),
      ),
    ),
  );
  return $plugins;
}
