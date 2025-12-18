Import("env")

def after_upload(source, target, env):
    import time
    time.sleep(2)

env.AddPostAction("upload", after_upload)
