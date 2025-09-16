<#
Lists all assets under /assets/ with their respective UUIDs
#>

$repo_top = git rev-parse --show-toplevel
$assets_dir = Join-Path $repo_top assets

$props = @(
    @{
        Name = 'ID'
        Expr = {
            $json = $_ | Get-Content | ConvertFrom-Json
            $json.id
        }
    }
    @{
        Name = 'Asset path'
        Expr = {
            $path = $_ | Resolve-Path -Relative -RelativeBasePath $assets_dir
            $path -replace '\\', '/' -replace '^./' -replace '\.meta$'
        }
    }
    @{
        Name = 'Type'
        Expr = {
            switch ($_.BaseName | Split-Path -Extension) {
                '.bind' { 'Input bindings' }
                '.grd' { 'Voxel grid' }
                '.pal' { 'World palette' }
                '.cubos' { 'Cubos scene' }
                Default { '???' }
            }
        }
    }
)

Get-ChildItem -Recurse $assets_dir -Filter *.meta |
Format-Table $props
