module "github-ci" {
  source                = "git@github.com:tundranerd/febio-ci-terraform"
  repo_name             = var.repo_name
  aws_secret_access_key = var.aws_secret_access_key
  aws_access_key_id     = var.aws_access_key_id
  gh_token              = var.gh_token
  repo_host             = var.repo_host
  repo_user             = var.repo_user
  repo_key              = var.repo_key
}
