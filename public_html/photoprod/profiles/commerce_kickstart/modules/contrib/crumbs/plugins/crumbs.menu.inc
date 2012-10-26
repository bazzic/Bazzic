<?php


/**
 * Implementation of hook_crumbs_plugins()
 */
function menu_crumbs_plugins($api) {
  $api->multiPlugin('hierarchy');
  $api->multiPlugin('link_title');
}


class menu_CrumbsMultiPlugin_hierarchy implements crumbs_MultiPlugin {

  function describe($api) {
    return menu_get_menus();
  }

  function disabledByDefault() {
    return array('*');
  }

  function findParent($path, $item) {
    $q = db_select('menu_links', 'parent');
    $q->innerJoin('menu_links', 'child', 'parent.mlid = child.plid');
    $q->addExpression('parent.link_path', 'parent_path');
    $q->addExpression('parent.menu_name', 'menu_name');
    $q->addExpression('parent.depth', 'depth');
    $q->condition('child.link_path', $path);
    // Ignore placeholder or separator menu items added by special_menu_items.
    $q->condition('parent.link_path', array('<nolink>', '<separator>'), 'NOT IN');
    // If we'd use both GROUP BY and parent.link_path, sqlsrv would complain,
    // because it would not know which row's link path to return in case of
    // duplicate.
    // There is no useful aggregate function for strings, unfortunately.
    // $q->groupBy('parent.menu_name');
    $candidates = array();
    foreach ($q->execute() as $row) {
      $candidates[$row->menu_name][$row->depth][] = $row->parent_path;
    }
    foreach ($candidates as &$menu_candidates) {
      ksort($menu_candidates);
      // pick the first array value, two times.
      // $menu_candidates is already a reference, so here we use it without '&'.
      foreach ($menu_candidates as $menu_candidates) break;
      foreach ($menu_candidates as $menu_candidates) break;
    }
    return $candidates;
  }
}


class menu_CrumbsMultiPlugin_link_title implements crumbs_MultiPlugin {

  function describe($api) {
    return menu_get_menus();
  }

  function disabledByDefault() {
    return array('*');
  }

  function findTitle($path, $item) {
    $q = db_select('menu_links', 'ml');
    $q->fields('ml', array('link_title', 'menu_name'));
    $q->condition('link_path', $path);
    $q->groupBy('menu_name');
    $titles = array();
    foreach ($q->execute() as $row) {
      $titles[$row->menu_name] = $row->link_title;
    }
    return $titles;
  }
}







