<?php


/**
 * Implements hook_crumbs_plugins()
 */
function pathauto_crumbs_plugins($api) {
  // TODO: Port to D7, or drop it.
  return;
  $api->monoPlugin();
}


/**
 * Implementation of class hook class_CrumbsParentFinder
 * on the behalf of path module.
 */
class pathauto_CrumbsMonoPlugin implements crumbs_MonoPlugin {

  function __construct() {
    module_load_include('inc', 'pathauto', 'pathauto');
  }

  function describe($api) {
    return t('Chop off the last fragment from the alias that pathauto would assign to this path. Warning: Poor performance.');
  }

  function disabledByDefault() {
    return array('');
  }

  function findParent__node_x($path, $item) {
    $node = $item['page_arguments'][0];
    if (is_object($node)) {
      $placeholders = pathauto_get_placeholders('node', $node);
      $alias = pathauto_create_alias('node', 'return', $placeholders, $path, $node->nid, $node->type, $node->language);
      return crumbs_reduce_path($alias, 1);
    }
  }
}







