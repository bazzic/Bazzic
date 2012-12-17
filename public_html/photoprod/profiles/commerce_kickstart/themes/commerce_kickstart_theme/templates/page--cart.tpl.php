<?php 
/**
 * @file
 * Alpha's theme implementation to display a single Drupal page.
 */
?>
<?php $colorbox = array_key_exists('iframe', $_GET) ? $_GET['iframe'] : FALSE; ?>
<div<?php print $attributes; ?>>
  <?php if (isset($page['header']) && !$colorbox) : ?>
    <?php print render($page['header']); ?>
  <?php endif; ?>
  
  <?php if (isset($page['content'])) : ?>
    <?php print render($page['content']); ?>
  <?php endif; ?>  
  
  <?php if (isset($page['footer'])&& !$colorbox) : ?>
    <?php print render($page['footer']); ?>
  <?php endif; ?>
</div>