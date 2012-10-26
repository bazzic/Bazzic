<?php


class crumbs_InvokeAction_preprocessPage implements crumbs_InvokeActionInterface_alter {

  protected $vars;

  function __construct(array &$vars) {
    $this->vars = &$vars;
  }

  /**
   * The point of the preprocessPage() method hook is a guaranteed execution
   * directly after crumbs_preprocess_page().
   */
  function invoke($plugin, $plugin_key) {
    if (method_exists($plugin, 'preprocessPage')) {
      $plugin->preprocessPage($this->vars);
    }
  }
}
