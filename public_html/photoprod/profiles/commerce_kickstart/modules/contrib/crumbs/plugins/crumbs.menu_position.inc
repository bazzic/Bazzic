<?php


/**
 * Implements hook_crumbs_plugins()
 */
function menu_position_crumbs_plugins($api) {

  // TODO:
  //   Finish this plugin.
  //   Problem: menu_position is not exactly designed to be used like this.
  //   Problem: MonoPlugin vs MultiPlugin? One instance per menu_position rule,
  //     or one instance for everything?
  return;

  // Retrieve the rules from the database. For speed, we don't call
  // menu_position_read_rules() and unserialize the conditions only if needed.
  $rules = db_query(
    'SELECT * FROM {menu_position_rules} WHERE enabled = :enabled ORDER BY weight, rid',
    array(':enabled' => 1)
  );

  foreach ($rules as $rule) {
    $api->monoPlugin($rule->rid, new _menu_position_CrumbsMonoPlugin($rule));
  }

  $api->disabledByDefault('*');
}


class _menu_position_CrumbsMonoPlugin implements crumbs_MonoPlugin {

  protected $rule;

  function __construct($rule) {
    $this->rule = $rule;
  }

  function describe($api) {
    return t("Use the rule configured with 'Menu position' module.");
  }

  function findParent__node_x($path, $item) {
    $node = $item['map'][1];
    $context = array(
      'path' => $path,
      'entity_type' => 'node',
      'bundle_name' => $node->type,
    );
    return $this->_findParent($context);
  }

  function findParent__user_x($path, $item) {
    $user = $item['map'][1];
    $context = array(
      'path' => $path,
      'entity_type' => 'user',
      'bundle_name' => 'user',
    );
    return $this->_findParent($context);
  }

  function findParent($path, $item) {
    $context = array(
      'path' => $path,
      'entity_type' => NULL,
      'bundle_name' => NULL,
    );
    return $this->_findParent($context);
  }

  protected function _findParent($context) {
    $this->_checkRule($context);
  }

  protected function _checkRule($context) {

    // Retrieve the list of menus the path is already in.
    $menu_names = db_query(
      'SELECT menu_name FROM {menu_links} WHERE link_path = :path',
      array(':path' => $context['path'])
    )->fetchCol();

    if (in_array($this->rule->menu_name, $menu_names)) {
      // If the page is already placed in the rule's menu, skip the rule.
      $rule_matches = FALSE;
      $set_breadcrumb = FALSE;
    }
    else {
      // A rule with no conditions always matches.
      $rule_matches = TRUE;
      // Go through each condition, ANDing each result.
      $this->rule->conditions = unserialize($this->rule->conditions);
      foreach ($this->rule->conditions as $plugin => $variables) {
        // Add the current rule and node to the callback's variables.
        $variables['rule'] = $this->rule;
        $variables['context'] = $context;
        // Find the plugin's callback function.
        $callback = menu_position_get_condition_callback($plugin);
        if ($callback) {
          // Check if this condition matches.
          $rule_matches = $callback($variables);
        }
        else {
          // If the callback cannot be found, the condition has failed.
          $rule_matches = FALSE;
        }
        // No need to check other conditions if this condition failed.
        if (!$rule_matches) {
          break;
        }
      }
    }

    // Let other modules manipulate the rule.
    drupal_alter('menu_position_rule', $this->rule, $context, $rule_matches, $set_breadcrumb);

    // We've found a matching rule.
    if ($rule_matches && menu_position_activate_rule($this->rule, $context, $set_breadcrumb)) {
      // Don't let other rules set the breadcrumb.
      $set_breadcrumb = FALSE;

      // Don't let other rules match against this rule's menu.
      $menu_names[] = $this->rule->menu_name;
    }
  }
}
